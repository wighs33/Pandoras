#include "GA_LevelUp.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"

#include "Interface/CharacterInterface.h"
#include "AttributeSet/BaseActorAttributes.h"

void UGA_LevelUp::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();

	if (!Avatar || !GetWorld())
	{
		// 안전하게 종료
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEnd*/true, /*bWasCancelled*/true);
		return;
	}

	// 어트리뷰트의 경험치가 레벨업요구치를 넘겼는 지 체크----------------------------------------------------------------------------
	UBaseActorAttributes* BaseAttr = nullptr;
	if (Avatar->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		BaseAttr = ICharacterInterface::Execute_GetBaseActorAttribute(Avatar);
	}

	bool bCanLevelUp = false;
	if (BaseAttr)
	{
		const float XP_CurrentValue = BaseAttr->XPPoints.GetCurrentValue();
		bCanLevelUp = (static_cast<double>(XP_CurrentValue) >= XPRequiredToLevelUp);
	}

	if (bCanLevelUp)
	{
		// 오너에 레벨업 GE 적용 --------------------------------------------------------------------------------------------
		if (LevelUpEffectClass)
		{
			BP_ApplyGameplayEffectToOwner(LevelUpEffectClass, /*Level*/1.f, /*Stacks*/1);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, /*bWasCancelled=*/false, /*bReplicateEnd=*/false);
}