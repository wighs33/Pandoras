// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_AttackBlockedReact.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UGA_AttackBlockedReact : public UGA_Pandoras
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> HitReact;
};
