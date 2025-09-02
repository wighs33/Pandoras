// 어빌리티 실행 태스크

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_ActivateAbility.generated.h"

UCLASS()
class PANDORAS_API UBTTask_ActivateAbility : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayTagContainer AbilityTags;

	UPROPERTY(BlueprintReadWrite, Category = "C++")
	int32 TargetActivateCount;

	UPROPERTY(BlueprintReadWrite, Category = "C++")
	int32 ActivatedCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 MaxActivateCount = 1;
};
