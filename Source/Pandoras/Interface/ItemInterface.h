// ������ �������̽�

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IItemInterface
{
	GENERATED_BODY()
	
public:
	// �� ���� ����Ʈ ���
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ToggleSwordTrail(bool Enable);

	// ���� ����Ʈ ���
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void TriggerBlood();

	// ������ �±� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "WeaponTag"))
	FGameplayTagContainer GetItemTag();

	// ������ Ÿ�� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "WeaponType"))
	EItem GetItemType();

	// ���� ����߸���
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Drop();
};
