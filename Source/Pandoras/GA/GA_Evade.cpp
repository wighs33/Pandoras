// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Evade.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"

void UGA_Evade::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// --- 회피 GE 적용 (회피 태그) -----------------------------
	BP_ApplyGameplayEffectToOwner(EvadeEffectClass, /*Level*/1, /*Stacks*/1);

	// --- 오너의 속도 10 아래인지 체크 --------------------------
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		K2_EndAbility();
		return;
	}

	const FVector Vel = Avatar->GetVelocity();
	const double Speed = static_cast<double>(Vel.Size());

	if (Speed < 10.0)
	{
		// --- 백스텝 애니메이션 --------------------------------
		UAbilityTask_PlayMontageAndWait* Task =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				/*OwningAbility*/this,
				/*TaskInstanceName*/NAME_None,
				/*몽타주*/EvadeBackMontage
			);

		if (Task)
		{
			Task->OnCompleted.AddDynamic(this, &UGA_Evade::OnMontageFinish);
			Task->OnInterrupted.AddDynamic(this, &UGA_Evade::OnMontageFinish);
			Task->OnCancelled.AddDynamic(this, &UGA_Evade::OnMontageFinish);
			Task->ReadyForActivation();
		}
	}
	else
	{
		// --- 오너는 이동하고 있는 방향으로 회전 -----------------
		Avatar->SetActorRotation(FRotator(0.f, Vel.Rotation().Yaw, 0.f));

		// --- 앞으로 구르기 애니메이션 ---------------------------
		UAbilityTask_PlayMontageAndWait* Task =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				/*OwningAbility*/this,
				/*TaskInstanceName*/NAME_None,
				/*몽타주*/EvadeRollForwardMontage
			);

		if (Task)
		{
			Task->OnCompleted.AddDynamic(this, &UGA_Evade::OnMontageFinish);
			Task->OnInterrupted.AddDynamic(this, &UGA_Evade::OnMontageFinish);
			Task->OnCancelled.AddDynamic(this, &UGA_Evade::OnMontageFinish);
			Task->ReadyForActivation();
		}
	}
}

void UGA_Evade::OnMontageFinish()
{
	K2_EndAbility();
}