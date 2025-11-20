#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_ParryReact.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UGA_ParryReact : public UGA_Pandoras
{
	GENERATED_BODY()
	
protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TObjectPtr<UAnimMontage> ParryReactMontage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
    TSubclassOf<UGameplayEffect> GE_StunAttack;
};
