// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_LockOn.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UGA_LockOn : public UGA_Pandoras
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnDelayCompleted();

	UFUNCTION()
	void Step_LockOnTick();

	UFUNCTION()
	void ScheduleDelay();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> LockOnEffectClass;
};
