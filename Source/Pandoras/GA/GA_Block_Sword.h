// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_Block_Sword.generated.h"


UCLASS()
class PANDORAS_API UGA_Block_Sword : public UGA_Pandoras
{
	GENERATED_BODY()
	
protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    UFUNCTION()
    void HandleCancel();

    void RemoveBlockEffectAndFinish();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TObjectPtr<UAnimMontage> BlockMontage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TSubclassOf<UGameplayEffect> GE_Block;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TObjectPtr<AActor> Sword = nullptr;
};
