// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA/GA_Pandoras.h"
#include "GA_Equip.generated.h"

class AItemBase;

UCLASS()
class PANDORAS_API UGA_Equip : public UGA_Pandoras
{
	GENERATED_BODY()
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<AItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayTag GameplayCueTag;
};
