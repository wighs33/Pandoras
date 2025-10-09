#include "GA_AttackBlockedReact.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimMontage.h"
#include "UObject/SoftObjectPtr.h"

void UGA_AttackBlockedReact::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// Mid부터 피격 애니메이션 재생--------------------------------------------------------------------------------
	UAbilityTask_PlayMontageAndWait* Task =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			HitReact,
			/*Rate*/ 1.0f,
			/*StartSectionName*/ FName(TEXT("Mid")),
			/*bStopWhenAbilityEnds*/ true,
			/*AnimRootMotionTranslationScale*/ 1.0f,
			/*StartTimeSeconds*/ 0.0f,
			/*bAllowInterruptAfterBlendOut*/ false
		);

	if (Task)
	{
		Task->ReadyForActivation();
	}

	// 딜레이 후 종료 ----------------------------------------------------------------------------
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName(TEXT("K2_EndAbility"));
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__; // 임의의 식별자

	UKismetSystemLibrary::Delay(this, /*Duration*/ 1.0f, LatentInfo);
}
