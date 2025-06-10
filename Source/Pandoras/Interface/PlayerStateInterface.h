// �÷��̾� ������Ʈ�� �������̽�

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerStateInterface.generated.h"

class UGA_Equip;

UINTERFACE(Blueprintable, BlueprintType)
class UPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IPlayerStateInterface
{
	GENERATED_BODY()

public:
	// Ÿ�� ���� �ƾ��� Ŭ���� �迭 ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	TArray<TSubclassOf<UGA_Equip>> GetItemAbilityClassesByType(EItem ItemType);

	// �迭�� ������ Ŭ���� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void AddItemClass(TSubclassOf<UGA_Equip> itemAbility);

	// �����۵� �Ѱ��� ��Ƽ� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	TArray<TSubclassOf<UGA_Equip>> GetAllItemAbilities();
};
