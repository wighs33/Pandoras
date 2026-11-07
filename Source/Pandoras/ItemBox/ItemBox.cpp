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
#include "Net/UnrealNetwork.h"

namespace
{
    // 뚜껑이 열린 뒤의 상대 위치
    const FVector LidOpenedRelativeLocation(-60.f, -60.f, 0.f);
    const FRotator LidOpenedRelativeRotation(0.f, 0.f, 0.f);
}

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


void AItemBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AItemBox, bCollected);
}

////////////////////////////////////////////////////////////////////////////////

bool AItemBox::Collect_Server_Validate(AActor* OwnerActor)
{
    return true;
}

// 아이템 획득 (클라-> 서버)
void AItemBox::Collect_Server_Implementation(AActor* OwnerActor)
{
    // --- 하나의 상자는 한 번만 획득 -----------------------------------------
    if (bCollected)
    {
        return;
    }

    BP_Collect_Server(OwnerActor);

    // BP_Collect_Server가 이미 멀티캐스트한 상자는 다시 보내지 않는다.
    if (!bCollected)
    {
        Collect_Multicast(OwnerActor);
    }
}

// 아이템 획득 (서버-> 모두)
void AItemBox::Collect_Multicast_Implementation(AActor* OwnerActor)
{
    // --- 하나의 상자는 한 번만 획득 -----------------------------------------
    if (bCollectStarted)
    {
        return;
    }
    bCollectStarted = true;

    // --- 획득 여부는 서버가 정해 복제한다 ---------------------------------------
    if (HasAuthority())
    {
        bCollected = true;
    }

    // 연출과 보상이 그래프에 모두 있는 상자는 C++ 처리까지 돌면 아이템이 두 번 지급된다.
    if (GetClass()->IsFunctionImplementedInScript(GET_FUNCTION_NAME_CHECKED(AItemBox, BP_Collect_Multicast)))
    {
        BP_Collect_Multicast(OwnerActor);
        return;
    }

    CachedOwnerActor = OwnerActor;

    // --- 이펙트 활성화 ---------------------------------------------------
    if (ChestFX)
    {
        ChestFX->Activate(true);
    }

    // --- 사운드 재생 ----------------------------------------------------
    if (OpenSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, OpenSound, GetActorLocation());
    }

    // --- 뚜껑 열기 -----------------------------------------------------------
    if (Lid)
    {
        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = this;
        LatentInfo.ExecutionFunction = FName(TEXT("OnLidOpened"));
        LatentInfo.Linkage = 0;
        LatentInfo.UUID = ++LidLatentUUID;

        UKismetSystemLibrary::MoveComponentTo(
            Lid,
            LidOpenedRelativeLocation,
            LidOpenedRelativeRotation,
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
        // --- Lid가 없으면 바로 다음 단계로 ------------------------------------
        OnLidOpened();
    }
}

// 멀티캐스트를 받지 못한 클라이언트(늦은 입장 등)는 복제값으로 상자 상태를 맞춘다
void AItemBox::OnRep_Collected()
{
    // 멀티캐스트로 여는 중이면 연출이 끝나면서 마무리한다.
    if (!bCollected || bCollectStarted)
    {
        return;
    }

    if (Lid)
    {
        Lid->SetRelativeLocationAndRotation(LidOpenedRelativeLocation, LidOpenedRelativeRotation);
    }

    HideBillboardAndDisableTrigger();
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