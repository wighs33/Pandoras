// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Pandoras.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/FactionsInterface.h"
#include "GameFramework/Character.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����RPC ���
bool UGA_Pandoras::ActivateAbilityOnServer_Validate(AActor* TargetActor, FGameplayTagContainer Tag)
{
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �±׷� �����Ƽ ���� (Ŭ��->����)
void UGA_Pandoras::ActivateAbilityOnServer_Implementation(AActor* TargetActor, FGameplayTagContainer Tags)
{
    if (!ensure(TargetActor)) return;

    // 1) TargetActor�κ��� Ability System Component ��������
    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    if (!ensure(ASC)) return;

    // 2) ���� �±� �����̳ʸ� �̿��� TryActivateAbilitiesByTag ȣ��
    //    bAllowRemoteActivation�� true�� �ϸ�, ������ Ȱ��ȭ ��û �� Ŭ���̾�Ʈ������ �������� Ȱ��ȭ�� �� �ֽ��ϴ�.
    bool bActivatedAny = ASC->TryActivateAbilitiesByTag(
        Tags,           // �������Ʈ ����� ��GameplayTagContainer�� ��°��� ����
        true            // bAllowRemoteActivation = true
    );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ����� �� ���
void UGA_Pandoras::GetClosestEnemy_Implementation(AActor*& ClosestEnemy, bool& bLeftOrRight)
{
	// 1) AvatarActor(���� ĳ����) ���
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		ClosestEnemy = nullptr;
		bLeftOrRight = true;
		return;
	}

	// 2) Avatar�� �Ĺ� ID ��������
	int32 OwnerFaction = 0;
	if (AvatarActor->GetClass()->ImplementsInterface(UFactionsInterface::StaticClass()))
	{
		OwnerFaction = IFactionsInterface::Execute_GetFactionId(AvatarActor);
	}

	// 3) FrontPoint ���: Avatar ��ġ + ���� ���� * 50
	const FVector AvatarLoc = AvatarActor->GetActorLocation();
	const FVector AvatarForward = AvatarActor->GetActorForwardVector();
	const FVector FrontPoint = AvatarLoc + AvatarForward * 50.0f;

	// 4) SphereOverlapActors ȣ�� (Pawn ä�θ� �˻�)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3); // Pawn �浹 ä��

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(
		this,                       // WorldContextObject
		FrontPoint,                 // Sphere ��ġ
		350.0f,                     // �ݰ�
		ObjectTypes,                // ObjectTypes (Pawn)
		AActor::StaticClass(),      // ActorClassFilter
		ActorsToIgnore,             // ������ ����
		OverlappedActors            // ��� �迭
	);

	// 5) �ʱ�ȭ
	AActor* BestEnemy = nullptr;
	double    BestDistance = 5'000'000.0; // �������Ʈ �ʱⰪ�� ����
	bool      BestIsLeft = true;

	// 6) OverlappedActors �߿��� ���� �Ĺ��� �ƴ� Pawn�� ���� �� �Ÿ� ��
	for (AActor* Other : OverlappedActors)
	{
		if (!Other) continue;

		// �Ĺ� üũ: IFactionsInterface
		if (Other->GetClass()->ImplementsInterface(UFactionsInterface::StaticClass()))
		{
			const int32 OtherFaction = IFactionsInterface::Execute_GetFactionId(Other);
			if (OtherFaction == OwnerFaction)
			{
				continue; // ���� �Ĺ��̸� �н�
			}
		}
		else
		{
			continue; // �������̽� ���� �� �Ǹ� �н�
		}

		// 7) FrontPoint �� Other ��ġ �� �Ÿ� ���
		const FVector OtherLoc = Other->GetActorLocation();
		const double  DistToFront = FVector::Dist(OtherLoc, FrontPoint);

		// 8) �ּڰ� ����
		if (DistToFront < BestDistance)
		{
			BestDistance = DistToFront;
			BestEnemy = Other;

			// 9) ����/������ ����
			const FVector AvatarRight = AvatarActor->GetActorRightVector();
			const FVector Offset = AvatarRight * 100.0f;
			const FVector RightPoint = FrontPoint + Offset;
			const FVector LeftPoint = FrontPoint - Offset;

			const double DistToRight = FVector::Dist(OtherLoc, RightPoint);
			const double DistToLeft = FVector::Dist(OtherLoc, LeftPoint);

			// �������Ʈ�� ����: (DistToRight - DistToLeft) > -10 �� true=����, false=������
			BestIsLeft = ((DistToRight - DistToLeft) > -10.0);
		}
	}

	// 10) out �Ķ���Ϳ� �� ����
	ClosestEnemy = BestEnemy;
	bLeftOrRight = BestIsLeft;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ĳ���� ���
ACharacter* UGA_Pandoras::GetCharacter_Implementation()
{
	// 1) AvatarActor ��������
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!ensure(AvatarActor)) return nullptr;

	// 2) ACharacter�� ĳ����
	ACharacter* AsChar = Cast<ACharacter>(AvatarActor);
	return AsChar;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����Ƽ �ο�
void UGA_Pandoras::GrantAbilities_Implementation()
{
    // 1) �� Ability�� �����ϴ� Actor(Avatar) ��������
    AActor* AvatarActor = GetAvatarActorFromActorInfo();
    if (!ensure(AvatarActor)) return;

    // 2) AvatarActor�� �پ� �ִ� ASC(AbilitySystemComponent) ��������
    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AvatarActor);
    if (!ensure(ASC)) return;

    // 3) GrantedAbilities �迭�� ��ȸ�ϸ鼭 �ϳ��� GiveAbility ȣ��
    for (TSubclassOf<UGameplayAbility> AbilityClass : GrantedAbilities)
    {
        if (!ensure(AbilityClass)) return;

        // Blueprint ��� K2_GiveAbility�� ������ ������ �ϵ��� Spec�� ����� ASC�� �ο�
        FGameplayAbilitySpec AbilitySpec(
            AbilityClass,       // �����Ƽ Ŭ����
            0,                  // Level (�������Ʈ ���������� 0���� �����Ǿ� �־����ϴ�)
            INDEX_NONE,         // InputID (-1, �� ���ε� ���� �ο�)
            this                // SourceObject (�� GA_Pandoras �ν��Ͻ�)
        );

        // ASC->GiveAbility �� ���������� K2_GiveAbility�� ȣ���� �Ͱ� ���� ����� ����ϴ�.
        ASC->GiveAbility(AbilitySpec);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ GE�� ���ʿ� ����
void UGA_Pandoras::ApplyGameplayEffects_Implementation()
{
    // 1) AbilityInfo���� ���� Actor�� ASC(AbilitySystemComponent)�� �����´�.
    UAbilitySystemComponent* ASC = nullptr;
    if (const FGameplayAbilityActorInfo* ActorInfo = GetCurrentActorInfo())
    {
        ASC = ActorInfo->AbilitySystemComponent.Get();
    }
    if (!ensure(ASC)) return;

    // 2) AppliedGameplayEffects �迭�� ��ȸ
    for (TSubclassOf<UGameplayEffect> GEClass : AppliedGameplayEffects)
    {
        if (!ensure(GEClass)) return;

        // 3) GameplayEffectSpecHandle�� ���� ��, ���� ���� ������ �����Ѵ�.
        //    Blueprint ���� ��� ��ApplyGameplayEffectToOwner�� �� Level=1, Stacks=1 ���� �������Ƿ� �״�� ����.
        const float EffectLevel = 1.0f;
        const int32 EffectStacks = 1;

        // 3-1) SpecHandle ���� : ������ �Ķ���ͷ� ����
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GEClass, EffectLevel);
        if (!ensure(SpecHandle.IsValid())) return;

        // 3-2) ���� ��(Effect Stacks) ����
		SpecHandle.Data->SetStackCount(EffectStacks);

        // 4) �����÷��� ����Ʈ�� ������(Self)������ ����
        FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        if (!ensure(ActiveHandle.WasSuccessfullyApplied())) return;
    }
}
