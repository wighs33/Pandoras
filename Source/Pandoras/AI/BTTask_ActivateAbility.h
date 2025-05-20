// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_ActivateAbility.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UBTTask_ActivateAbility : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
	//virtual EBTNodeResult::Type ExecuteTask(
	//	UBehaviorTreeComponent& OwnerComp,
	//	uint8* NodeMemory
	//) override

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayTagContainer AbilityTags;

	UPROPERTY(BlueprintReadWrite, Category = "C++")
	int32 TargetActivateCount;

	UPROPERTY(BlueprintReadWrite, Category = "C++")
	int32 ActivatedCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 MaxActivateCount;
};
