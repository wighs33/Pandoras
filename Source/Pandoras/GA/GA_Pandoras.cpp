// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Pandoras.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/FactionsInterface.h"
#include "GameFramework/Character.h"

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

    // 1) TargetActor로부터 Ability System Component 가져오기
    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!ensure(ASC)) return;

    // 2) 받은 태그 컨테이너를 이용해 TryActivateAbilitiesByTag 호출
    //    bAllowRemoteActivation을 true로 하면, 서버가 활성화 요청 시 클라이언트에서도 원격으로 활성화될 수 있습니다.
    bool bActivatedAny = ASC->TryActivateAbilitiesByTag(
        Tags,           // 블루프린트 노드의 “GameplayTagContainer” 출력값과 동일
        true            // bAllowRemoteActivation = true
    );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 가장 가까운 적 얻기
void UGA_Pandoras::GetClosestEnemy_Implementation(AActor*& ClosestEnemy, bool& bLeftOrRight)
{
	// 1) AvatarActor(소유 캐릭터) 얻기
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		ClosestEnemy = nullptr;
		bLeftOrRight = true;
		return;
	}

	// 2) Avatar의 파벌 ID 가져오기
	int32 OwnerFaction = 0;
	if (AvatarActor->GetClass()->ImplementsInterface(UFactionsInterface::StaticClass()))
	{
		OwnerFaction = IFactionsInterface::Execute_GetFactionId(AvatarActor);
	}

	// 3) FrontPoint 계산: Avatar 위치 + 전방 벡터 * 50
	const FVector AvatarLoc = AvatarActor->GetActorLocation();
	const FVector AvatarForward = AvatarActor->GetActorForwardVector();
	const FVector FrontPoint = AvatarLoc + AvatarForward * 50.0f;

	// 4) SphereOverlapActors 호출 (Pawn 채널만 검사)
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

	// 5) 초기화
	AActor* BestEnemy = nullptr;
	double    BestDistance = 5'000'000.0; // 블루프린트 초기값과 동일
	bool      BestIsLeft = true;

	// 6) OverlappedActors 중에서 같은 파벌이 아닌 Pawn만 필터 → 거리 비교
	for (AActor* Other : OverlappedActors)
	{
		if (!Other) continue;

		// 파벌 체크: IFactionsInterface
		if (Other->GetClass()->ImplementsInterface(UFactionsInterface::StaticClass()))
		{
			const int32 OtherFaction = IFactionsInterface::Execute_GetFactionId(Other);
			if (OtherFaction == OwnerFaction)
			{
				continue; // 같은 파벌이면 패스
			}
		}
		else
		{
			continue; // 인터페이스 구현 안 되면 패스
		}

		// 7) FrontPoint ↔ Other 위치 간 거리 계산
		const FVector OtherLoc = Other->GetActorLocation();
		const double  DistToFront = FVector::Dist(OtherLoc, FrontPoint);

		// 8) 최솟값 갱신
		if (DistToFront < BestDistance)
		{
			BestDistance = DistToFront;
			BestEnemy = Other;

			// 9) 왼쪽/오른쪽 판정
			const FVector AvatarRight = AvatarActor->GetActorRightVector();
			const FVector Offset = AvatarRight * 100.0f;
			const FVector RightPoint = FrontPoint + Offset;
			const FVector LeftPoint = FrontPoint - Offset;

			const double DistToRight = FVector::Dist(OtherLoc, RightPoint);
			const double DistToLeft = FVector::Dist(OtherLoc, LeftPoint);

			// 블루프린트와 동일: (DistToRight - DistToLeft) > -10 → true=왼쪽, false=오른쪽
			BestIsLeft = ((DistToRight - DistToLeft) > -10.0);
		}
	}

	// 10) out 파라미터에 값 전달
	ClosestEnemy = BestEnemy;
	bLeftOrRight = BestIsLeft;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 캐릭터 얻기
ACharacter* UGA_Pandoras::GetCharacter_Implementation()
{
	// 1) AvatarActor 가져오기
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!ensure(AvatarActor)) return nullptr;

	// 2) ACharacter로 캐스팅
	ACharacter* AsChar = Cast<ACharacter>(AvatarActor);
	return AsChar;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 어빌리티 부여
void UGA_Pandoras::GrantAbilities_Implementation()
{
    // 1) 이 Ability를 실행하는 Actor(Avatar) 가져오기
    AActor* AvatarActor = GetAvatarActorFromActorInfo();
    if (!ensure(AvatarActor)) return;

    // 2) AvatarActor에 붙어 있는 ASC(AbilitySystemComponent) 가져오기
    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AvatarActor);
    if (!ensure(ASC)) return;

    // 3) GrantedAbilities 배열을 순회하면서 하나씩 GiveAbility 호출
    for (TSubclassOf<UGameplayAbility> AbilityClass : GrantedAbilities)
    {
        if (!ensure(AbilityClass)) return;

        // Blueprint 노드 K2_GiveAbility과 동일한 동작을 하도록 Spec을 만들어 ASC에 부여
        FGameplayAbilitySpec AbilitySpec(
            AbilityClass,       // 어빌리티 클래스
            0,                  // Level (블루프린트 예제에서는 0으로 설정되어 있었습니다)
            INDEX_NONE,         // InputID (-1, 즉 바인딩 없이 부여)
            this                // SourceObject (이 GA_Pandoras 인스턴스)
        );

        // ASC->GiveAbility 은 실질적으로 K2_GiveAbility를 호출한 것과 같은 결과를 만듭니다.
        ASC->GiveAbility(AbilitySpec);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 지정된 GE들 오너에 적용
void UGA_Pandoras::ApplyGameplayEffects_Implementation()
{
    // 1) AbilityInfo에서 오너 Actor의 ASC(AbilitySystemComponent)를 가져온다.
    UAbilitySystemComponent* ASC = nullptr;
    if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
    {
        ASC = ActorInfo->AbilitySystemComponent.Get();
    }
    if (!ensure(ASC)) return;

    // 2) AppliedGameplayEffects 배열을 순회
    for (TSubclassOf<UGameplayEffect> GEClass : AppliedGameplayEffects)
    {
        if (!ensure(GEClass)) return;

        // 3) GameplayEffectSpecHandle을 만든 뒤, 스택 수와 레벨을 설정한다.
        //    Blueprint 에서 노드 “ApplyGameplayEffectToOwner” 에 Level=1, Stacks=1 으로 놓았으므로 그대로 적용.
        const float EffectLevel = 1.0f;
        const int32 EffectStacks = 1;

        // 3-1) SpecHandle 생성 : 레벨을 파라미터로 전달
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GEClass, EffectLevel);
        if (!ensure(SpecHandle.IsValid())) return;

        // 3-2) 스택 수(Effect Stacks) 설정
		SpecHandle.Data->SetStackCount(EffectStacks);

        // 4) 게임플레이 이펙트를 “오너(Self)”에게 적용
        FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        if (!ensure(ActiveHandle.WasSuccessfullyApplied())) return;
    }
}
