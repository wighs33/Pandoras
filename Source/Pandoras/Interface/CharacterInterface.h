// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Common/Structs.h"
#include "Common/Enums.h"
#include "CharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API ICharacterInterface
{
	GENERATED_BODY()

public:
	// 몽타주를 동기화하며 실행
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void PlayMontageReplicated(UAnimMontage* AnimMontage, float InPlayRate = 1.0, FName StartSectionName = TEXT("None"));

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ShowCharacterNotification(FNotification NotificationData);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	bool IsAlive();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void SetLockTarget(AActor* LockTarget);						

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ClearLockTarget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Die();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "Player"))
	bool IsPlayer();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "LockedEnemy"))
	AActor* GetLockedEnemy();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "Stamina"))
	float GetStamina();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "Mode"))
	ECustomMovementMode GetMovementMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void SetMovementMode(ECustomMovementMode MovementMode);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ResetMovementMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void NotifyFootstep(bool LeftOrRight);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void LockOffEnemy();
};
