// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/PlayerStateInterface.h"
#include "PandorasPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PANDORAS_API APandorasPlayerState : public APlayerState, public IPlayerStateInterface
{
	GENERATED_BODY()
	
protected:
	virtual TArray<TSubclassOf<AItemBase>> GetItemClasses() override;
	virtual void AddItemClass(TSubclassOf<AItemBase> item) override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AItemBase>> ItemClasses;
};
