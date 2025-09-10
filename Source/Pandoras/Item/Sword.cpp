// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Sword.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Common/Structs.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagAssetInterface.h"

ASword::ASword()
	: AItemBase()
{
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	BlockBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockBox"));

	HitBox->SetupAttachment(SK_Item);
	BlockBox->SetupAttachment(SK_Item);
}

void ASword::BeginPlay()
{
    Super::BeginPlay();

    // 히트박스 감지 끄기
    IItemInterface::Execute_ToggleHitBox(this, /*bEnable=*/false);

    // DT_Item에서 자신의 클래스명으로 Row 검색
    if (!SCREEN_WARN(DT_Item)) return;

    // C++: 클래스 네임에서 "_C" 제거
    FString RowString = GetClass()->GetName();        // 예: BP_Sword_C
    RowString.RemoveFromEnd(TEXT("_C"));              // -> BP_Sword
    const FName RowName(*RowString);

    const FItemData* Row = DT_Item->FindRow<FItemData>(RowName, TEXT("ASword::BeginPlay"));
    if (!SCREEN_WARN(Row)) return;

    // WeaponStats["Attack"]를 찾고 DamageAmount = -Attack 로 세팅
    const int32* AttackPtr = Row->WeaponProperties.WeaponStats.Find(TEXT("Attack"));
    if (!SCREEN_WARN(AttackPtr)) return;
	DamageAmount = -static_cast<float>(*AttackPtr);
}

void ASword::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 트리거 바인딩
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnHitBoxBeginOverlap);
	BlockBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnBlockBoxBeginOverlap);
}


// 이 클래스가 네트워크에 올라가는 시점에 호출
void ASword::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ItemClasses 변수를 복제 목록에 추가
	DOREPLIFETIME(ASword, TrailEnabled);
	DOREPLIFETIME(ASword, BloodTriggered);
}

void ASword::ToggleHitBox_Implementation(bool bEnable)
{
	if (HitBox)
	{
		const ECollisionEnabled::Type NewType =
			bEnable ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;

		HitBox->SetCollisionEnabled(NewType);
	}

	// 피격된 액터 리스트 비우기
	AttackedActors.Empty();
}

void ASword::ToggleBlockBox_Implementation(bool bEnable)
{
	if (BlockBox)
	{
		const ECollisionEnabled::Type NewType =
			bEnable ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;

		BlockBox->SetCollisionEnabled(NewType);
	}
}

void ASword::Drop_Implementation()
{
    if (!SCREEN_WARN(SK_Item))return;

    // 분리
    SK_Item->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

    // 충돌 프로필 변경
    SK_Item->SetCollisionProfileName(TEXT("BlockAll"), /*bUpdateOverlaps=*/true);

    // 충돌 활성화
    SK_Item->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

    // 물리 시뮬레이션 켜기
    SK_Item->SetSimulatePhysics(true);
}

void ASword::ApplyDamageAndStun_Implementation()
{
    if (!SCREEN_WARN(HitActor)) return;

    // Target ASC (피격자)
    UAbilitySystemComponent* TargetASC =
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
    if (!SCREEN_WARN(TargetASC)) return;

    // Source ASC (공격자: 검이 붙어있는 액터)
    AActor* InstigatorActor = GetAttachParentActor();
    UAbilitySystemComponent* SourceASC =
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor);

    // === Damage: MakeOutgoingSpec( GE_Damage_(lite/charged) ) ================================================
    const TSubclassOf<UGameplayEffect> DamageGEClass =
        WeaponCharged ? GE_Damage_Charged : GE_Damage_lite;

    if (SourceASC && *DamageGEClass)
    {
        FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
        Ctx.AddSourceObject(this);

        FGameplayEffectSpecHandle SpecHandle =
            SourceASC->MakeOutgoingSpec(DamageGEClass, /*Level*/0.f, Ctx);

        if (SpecHandle.IsValid())
        {
            // AssignTagSetByCallerMagnitude( DamageTag, DamageAmount )
            UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
                SpecHandle, DamageTag, DamageAmount);

            SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
        }
    }

    // === Stun: Evading 이면 패스, 아니면 GE_Stun 적용 ===============================================================
    const FGameplayTag EvadingTag = FGameplayTag::RequestGameplayTag(TEXT("Character.State.Evading"));
    FGameplayTagContainer EvadingContainer; EvadingContainer.AddTag(EvadingTag);

    bool bIsEvading = TargetASC->HasAnyMatchingGameplayTags(EvadingContainer);
    if (!bIsEvading && *GE_Stun)
    {
        FGameplayEffectContextHandle StunCtx = TargetASC->MakeEffectContext();
        StunCtx.AddSourceObject(this);

        TargetASC->BP_ApplyGameplayEffectToTarget(GE_Stun, TargetASC, /*Level*/0.f, StunCtx);
    }
}