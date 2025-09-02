// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_CheckTarget.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API UBTTask_CheckTarget : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FBlackboardKeySelector TargetActor;
};
