#include "ItemBox/ItemBox.h"

#include "Components/BoxComponent.h"
#include "Components/MaterialBillboardComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

#include "GA/GA_Equip.h"
#include "Item/ItemBase.h"
#include "Interface/ItemWielderInterface.h"
#include "Interface/CharacterInterface.h"

// 생성자
// 리플리케이션 허용
AItemBox::AItemBox()
{
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    Chest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Chest"));
    PlayerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerCollider"));
    Lid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Lid"));
    MaterialBillboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboard"));
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
    ChestFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_Chest"));

    RootComponent = DefaultSceneRoot;
    Chest->SetupAttachment(DefaultSceneRoot);
    PlayerCollider->SetupAttachment(Chest);
    Lid->SetupAttachment(Chest);
    MaterialBillboard->SetupAttachment(Chest);
    Widget->SetupAttachment(MaterialBillboard);
    ChestFX->SetupAttachment(Chest);

	bReplicates = true;
}

void AItemBox::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // 캡슐 클릭 시점 바인딩
    PlayerCollider->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnColliderBeginOverlap);
    PlayerCollider->OnComponentEndOverlap.AddDynamic(this, &AItemBox::OnColliderEndOverlap);
}

// 플레이 시작
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}


////////////////////////////////////////////////////////////////////////////////

bool AItemBox::Collect_Server_Validate(AActor* OwnerActor)
{
    return true;
}

// 아이템 획득 (클라-> 서버)
void AItemBox::Collect_Server_Implementation(AActor* OwnerActor)
{
    Collect_Multicast(OwnerActor);
}

// 아이템 획득 (서버-> 모두)
void AItemBox::Collect_Multicast_Implementation(AActor* OwnerActor)
{
    //BP_Collect_Multicast(OwnerActor);

     // DoOnce: 하나의 상자는 한 번만 획득
    if (bCollected)
    {
        return;
    }
    bCollected = true;

    CachedOwnerActor = OwnerActor;

    // 이펙트 활성화
    if (ChestFX)
    {
        ChestFX->Activate(true);
    }

    // 사운드 재생
    if (OpenSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
    }

    // 뚜껑 열기: MoveComponentTo (0.4s, Ease In/Out)
    if (Lid)
    {
        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = this;
        LatentInfo.ExecutionFunction = FName(TEXT("OnLidOpened"));
        LatentInfo.Linkage = 0;
        LatentInfo.UUID = ++LidLatentUUID;

        const FVector  TargetRelLoc(-60.f, -60.f, 0.f);
        const FRotator TargetRelRot = FRotator::ZeroRotator;

        UKismetSystemLibrary::MoveComponentTo(
            Lid,
            TargetRelLoc,
            TargetRelRot,
            /*bEaseOut*/ true,
            /*bEaseIn*/  true,
            /*OverTime*/ 0.4f,
            /*bForceShortestRotationPath*/ false,
            EMoveComponentAction::Move,
            LatentInfo
        );
    }
    else
    {
        // Lid가 없으면 바로 다음 단계로
        OnLidOpened();
    }
}

// MoveComponentTo 완료 후 실행
void AItemBox::OnLidOpened()
{
    if (HasAuthority())
    {
        // ASC 유효성 체크 후 아이템 지급 + 알림
        GiveItemsAndNotify(CachedOwnerActor.Get());

        // 경험치 GE 적용
        if (AActor* TargetActor = CachedOwnerActor.Get())
        {
            if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor))
            {
                if (MinorExperienceEffect)
                {
                    // BP_ApplyGameplayEffectToSelf와 동일한 호출
                    ASC->BP_ApplyGameplayEffectToSelf(MinorExperienceEffect, /*Level*/ 0.f, ASC->MakeEffectContext());
                }
            }
        }
    }

    // Remote와 Authority 공통 최종 처리:
    // 빌보드 시각화 끄기 + 충돌 트리거 끄기
    HideBillboardAndDisableTrigger();
}

// ASC 유효성 통과 시: ForEach(ItemAbilityClasses) -> PlayerState 저장 + HUD 알림
void AItemBox::GiveItemsAndNotify(AActor* TargetActor)
{
    if (!TargetActor)
    {
        return;
    }

    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
    if (!ASC)
    {
        return;
    }

    for (const TSubclassOf<UGA_Equip>& ItemGAClass : ItemAbilityClasses)
    {
        if (!ItemGAClass)
        {
            continue;
        }

        // 플레이어 스테이트에 아이템 데이터 저장
        IItemWielderInterface::Execute_AddItemToPlayerState(TargetActor, ItemGAClass);

        // HUD 알림 데이터 구성
        FString     ItemName = TEXT("");
        UTexture2D* ItemIcon = nullptr;

        if (const UGA_Equip* EquipCDO = ItemGAClass->GetDefaultObject<UGA_Equip>())
        {
            const TSubclassOf<AItemBase> ItemClass = EquipCDO->GetItemClass();
            if (ItemClass)
            {
                if (const AItemBase* ItemCDO = ItemClass->GetDefaultObject<AItemBase>())
                {
                    ItemName = ItemCDO->GetName();
                    ItemIcon = ItemCDO->GetIcon();
                }
            }
        }

        FNotification Noti;
        Noti.Text = ItemName + TEXT(" Collected");
        Noti.Icon = ItemIcon;

        // 캐릭터 인터페이스로 HUD의 알림 표시
        ICharacterInterface::Execute_ShowCharacterNotification(TargetActor, Noti);
    }
}

// 빌보드 숨기고, 플레이어 콜라이더 비활성화
void AItemBox::HideBillboardAndDisableTrigger()
{
    if (MaterialBillboard)
    {
        MaterialBillboard->SetVisibility(false, /*bPropagateToChildren*/ true);
    }
    if (PlayerCollider)
    {
        PlayerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}