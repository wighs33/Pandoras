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


	// 변수 복제를 위해 반드시 GetLifetimeReplicatedProps 를 오버라이드
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AItemBase>> ItemClasses;
};
