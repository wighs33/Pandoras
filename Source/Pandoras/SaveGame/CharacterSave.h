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
	// �ο��� �����Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	// ������ ���� �����Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGA_Equip>> InvectoryItemAbilities;

	// ����� ��Ʈ����Ʈ ��ġ ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TMap<FGameplayAttribute, float> AttributeMap;

};
