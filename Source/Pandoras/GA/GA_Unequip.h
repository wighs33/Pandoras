// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_Unequip.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UGA_Unequip : public UGA_Pandoras
{
	GENERATED_BODY()

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	FGameplayTagContainer WeaponTags;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
    FGameplayTag GameplayCueTag;
};
