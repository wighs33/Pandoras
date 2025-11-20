// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_FinishAttack_Sword.generated.h"

class UInputAction;

UCLASS()
class PANDORAS_API UGA_FinishAttack_Sword : public UGA_Pandoras
{
	GENERATED_BODY()

public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

protected:
    void RunNormal();

    void RunFromEvent(const FGameplayEventData* EventData);

    UFUNCTION()
    void ShowBlood(FGameplayEventData Payload);

    UFUNCTION()
    void Die(FGameplayEventData Payload);

    UFUNCTION()
    void OnMontageFinish();

    UFUNCTION()
    bool CheckConditions();

    UFUNCTION()
    bool CheckVictim() const;

	UFUNCTION()
	void ShowTip();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TArray<struct FPairedMontages> FinishAttackMontages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TSubclassOf<class UGameplayEffect> GE_DrainStamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TSubclassOf<class UGameplayEffect> GE_MinorExperienceClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TObjectPtr<AActor> Victim = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TObjectPtr<UInputAction> FinishAttackAction = nullptr;
};
