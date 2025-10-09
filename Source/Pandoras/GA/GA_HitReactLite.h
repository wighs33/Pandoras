// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_HitReactLite.generated.h"

UCLASS()
class PANDORAS_API UGA_HitReactLite : public UGA_Pandoras
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;


private:
	UFUNCTION() void OnRotateToAttackerCompleted();
	UFUNCTION() void OnDeath();
	UFUNCTION() void ResetTimeAndEnd();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> Attacker = nullptr;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> Reaction_Blocking;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> Reaction_NonBlocking;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> AddStaminaEffectClass;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> MinorExperienceEffectClass;
};
