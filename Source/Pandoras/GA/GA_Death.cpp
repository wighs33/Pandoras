#include "GA_Death.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Actor.h"

#include "Interface/CharacterInterface.h"

void UGA_Death::ActivateAbility(
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

	// 오너의 사망처리 ----------------------------------------------------------------------------------------
	if (Avatar->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		ICharacterInterface::Execute_Die(Avatar);
	}

	// 오너에 공격 중 GE 적용 (공격 중 태그)--------------------------------------------------------------------
	if (DeathEffectClass)
	{
		BP_ApplyGameplayEffectToOwner(DeathEffectClass, /*Level*/1.f, /*Stacks*/1);
	}

	// 종료 -----------------------------------------------------------------------------------------------------
	EndAbility(Handle, ActorInfo, ActivationInfo, /*bWasCancelled=*/false, /*bReplicateEnd=*/false);
}
