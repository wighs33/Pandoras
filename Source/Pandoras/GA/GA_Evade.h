#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Evade.generated.h"

UCLASS()
class PANDORAS_API UGA_Evade : public UGA_Pandoras
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageFinish();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> EvadeEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UAnimMontage* EvadeBackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UAnimMontage* EvadeRollForwardMontage = nullptr;
};
