// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Common/Structs.h"
#include "Common/Enums.h"
#include "CharacterInterface.generated.h"

class UBaseActorAttributes;

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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void PlayMontageReplicated(UAnimMontage* AnimMontage, float InPlayRate = 1.0, FName StartSectionName = TEXT("None"));

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ShowCharacterNotification(FNotification NotificationData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	bool IsAlive();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void SetLockTarget(AActor* LockTarget);						

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void ClearLockTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void Die();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "Player"))
	bool IsPlayer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "Player"))
	bool IsLocalCharacter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "LockedEnemy"))
	AActor* GetLockedEnemy();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "Stamina"))
	float GetStamina();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "Mode"))
	ECustomMovementMode GetMovementMode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void SetMovementMode(ECustomMovementMode MovementMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "Mode"))
	void ResetMovementMode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void NotifyFootstep();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void LockOffEnemy();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void SlowDown(float Rate = 0.2f, float Duration = 0.4f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	const UBaseActorAttributes* GetBaseActorAttribute();
};
