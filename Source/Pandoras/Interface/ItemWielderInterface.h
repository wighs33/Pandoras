// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemWielderInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UItemWielderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PANDORAS_API IItemWielderInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void CollectItem(AActor* item) = 0;
};
