// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_TakeDown_Sword.generated.h"

class UInputAction;

UCLASS()
class PANDORAS_API UGA_TakeDown_Sword : public UGA_Pandoras
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

    bool EquipWeapon();

    UFUNCTION()
    void ShowBlood(FGameplayEventData Payload);

    UFUNCTION()
    void Die(FGameplayEventData Payload);

    UFUNCTION()
    void OnMontageFinish();

    UFUNCTION()
    bool CheckVictim() const;

	UFUNCTION()
	void ShowTip();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TArray<struct FPairedMontages> FinishAttackMontages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TSubclassOf<class UGameplayEffect> GE_MinorExperienceClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    int32 AttackSeed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TObjectPtr<AActor> Victim = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TObjectPtr<UInputAction> FinishAttackAction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    FGameplayTag WeaponTag;
};
