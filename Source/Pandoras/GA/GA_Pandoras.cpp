// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Pandoras.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"

#include "Interface/FactionsInterface.h"


// 리플리케이션 설정
UGA_Pandoras::UGA_Pandoras()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 서버RPC 허용
bool UGA_Pandoras::ActivateAbilityOnServer_Validate(AActor* TargetActor, FGameplayTagContainer Tag)
{
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 태그로 어빌리티 실행 (클라->서버)
void UGA_Pandoras::ActivateAbilityOnServer_Implementation(AActor* TargetActor, FGameplayTagContainer Tags)
{
    if (!ensure(TargetActor)) return;

    // TargetActor로부터 ASC 가져오기
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
    if (!ensure(ASC)) return;

    // 받은 태그 컨테이너를 이용해 어빌리티 활성화
    ASC->TryActivateAbilitiesByTag(
        Tags,
        true
    );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 가장 가까운 적 얻기
void UGA_Pandoras::GetClosestEnemy_Implementation(AActor*& ClosestEnemy, bool& bLeftOrRight)
{
	// --- 오너 얻기 -----------------------
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		ClosestEnemy = nullptr;
		bLeftOrRight = true;
		return;
	}

	// --- 오너의 파벌 ID 가져오기 -----------------------------------
	int32 OwnerFaction = 0;
	if (AvatarActor->GetClass()->ImplementsInterface(UFactionsInterface::StaticClass()))
	{
		OwnerFaction = IFactionsInterface::Execute_GetFactionId(AvatarActor);
	}

	// --- FrontPoint 계산: 오너 위치 + 전방 벡터 * 50 --------------------
	const FVector AvatarLoc = AvatarActor->GetActorLocation();
	const FVector AvatarForward = AvatarActor->GetActorForwardVector();
	const FVector FrontPoint = AvatarLoc + AvatarForward * 50.0f;

	// --- SphereOverlapActors 호출 (Pawn 채널만 검사) ------------------------
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3); // Pawn 충돌 채널

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(
		this,                       // WorldContextObject
		FrontPoint,                 // Sphere 위치
		350.0f,                     // 반경
		ObjectTypes,                // ObjectTypes (Pawn)
		AActor::StaticClass(),      // ActorClassFilter
		ActorsToIgnore,             // 무시할 액터
		OverlappedActors            // 결과 배열
	);

	// --- 초기화 -------------------------------------------------------
	AActor* BestEnemy = nullptr;
	double    BestDistance = 5'000'000.0; // 블루프린트 초기값과 동일
	bool      BestIsLeft = true;

	// --- OverlappedActors 중에서 같은 파벌이 아닌 Pawn만 필터 -----------------------
	for (AActor* Other : OverlappedActors)
	{
		if (!Other) continue;

		// --- 파벌 체크: IFactionsInterface -----------------------------------------
		const int32 OtherFaction = IFactionsInterface::Execute_GetFactionId(Other);
		if (OtherFaction == OwnerFaction)
		{
			continue; // 같은 파벌이면 패스
		}

		// --- FrontPoint <-> Other 위치 간 거리 계산 ------------------------------------
		const FVector OtherLoc = Other->GetActorLocation();
		const double  DistToFront = FVector::Dist(OtherLoc, FrontPoint);

		// --- 최솟값 갱신 --------------------------
		if (DistToFront < BestDistance)
		{
			BestDistance = DistToFront;
			BestEnemy = Other;

			// --- 왼쪽/오른쪽 판정 ------------------------------------------
			const FVector AvatarRight = AvatarActor->GetActorRightVector();
			const FVector Offset = AvatarRight * 100.0f;
			const FVector RightPoint = FrontPoint + Offset;
			const FVector LeftPoint = FrontPoint - Offset;

			const double DistToRight = FVector::Dist(OtherLoc, RightPoint);
			const double DistToLeft = FVector::Dist(OtherLoc, LeftPoint);

			// true=왼쪽, false=오른쪽
			BestIsLeft = ((DistToRight - DistToLeft) > -10.0);
		}
	}

	// --- out 파라미터에 값 전달 ------------------------------
	ClosestEnemy = BestEnemy;
	bLeftOrRight = BestIsLeft;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 캐릭터 얻기
ACharacter* UGA_Pandoras::GetCharacter_Implementation()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!ensure(AvatarActor)) return nullptr;

	ACharacter* AsChar = Cast<ACharacter>(AvatarActor);
	return AsChar;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 어빌리티 부여
void UGA_Pandoras::GrantAbilities_Implementation()
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ensure(ASC)) return;

    for (TSubclassOf<UGameplayAbility> AbilityClass : GrantedAbilities)
    {
        if (!ensure(AbilityClass)) return;

        FGameplayAbilitySpec AbilitySpec(
            AbilityClass,       // 어빌리티 클래스
            0, 
            INDEX_NONE,
            this
        );

        ASC->GiveAbility(AbilitySpec);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 지정된 GE들 오너에 적용
void UGA_Pandoras::ApplyGameplayEffects_Implementation()
{
    UAbilitySystemComponent* ASC = nullptr;
    if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
    {
        ASC = ActorInfo->AbilitySystemComponent.Get();
    }
    if (!ensure(ASC)) return;

    for (TSubclassOf<UGameplayEffect> GEClass : AppliedGameplayEffects)
    {
        if (!ensure(GEClass)) return;


        const float EffectLevel = 1.0f;
        const int32 EffectStacks = 1;

        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GEClass, EffectLevel);
        if (!ensure(SpecHandle.IsValid())) return;

		SpecHandle.Data->SetStackCount(EffectStacks);

        FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        if (!ensure(ActiveHandle.WasSuccessfullyApplied())) return;
    }
}


void UGA_Pandoras::ApplyGameplayEffectToOwnerInCode_Implementation(TSubclassOf<class UGameplayEffect> GE)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	Ctx.AddSourceObject(GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GE, 1.f, Ctx);
	TSharedPtr<FGameplayEffectSpec> Spec = SpecHandle.Data;

	ASC->ApplyGameplayEffectSpecToSelf(*Spec);
}
