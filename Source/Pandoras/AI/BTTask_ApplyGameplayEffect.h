// GE 적용하는 태스크

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_ApplyGameplayEffect.generated.h"

class UGameplayEffect;

UCLASS()
class PANDORAS_API UBTTask_ApplyGameplayEffect : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Probability;
};
