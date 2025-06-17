// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AttributeSet.h"
#include "CharacterSave.generated.h"

class UGameplayAbility;
class UGA_Equip;

UCLASS()
class PANDORAS_API UCharacterSave : public USaveGame
{
	GENERATED_BODY()
	
protected:
	// 부여된 어빌리티
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	// 아이템 장착 어빌리티
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGA_Equip>> InvectoryItemAbilities;

	// 저장된 어트리뷰트 수치 리스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TMap<FGameplayAttribute, float> AttributeMap;

};
