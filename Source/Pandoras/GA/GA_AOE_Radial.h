#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_AOE_Radial.generated.h"

class AAreaOfEffect_RadialImpact;

UCLASS()
class PANDORAS_API UGA_AOE_Radial : public UGA_Pandoras
{
	GENERATED_BODY()

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
	void OnMontageFinished();

	UFUNCTION()
	void OnLaunchEffectEventReceived(FGameplayEventData Payload);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<AAreaOfEffect_RadialImpact> AOEImpactClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TObjectPtr<UAnimMontage> AOEMontage = nullptr;
};
