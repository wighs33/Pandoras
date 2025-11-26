#pragma once

#include "CoreMinimal.h"
#include "GA/GA_Pandoras.h"
#include "GA_LevelUp.generated.h"


UCLASS()
class PANDORAS_API UGA_LevelUp : public UGA_Pandoras
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
	TSubclassOf<UGameplayEffect> LevelUpEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float XPRequiredToLevelUp = 1000;
};
