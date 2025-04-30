// ĳ������ ������ ��� ���� �������̽�

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemWielderInterface.generated.h"

class AItemBase;

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ForceUnequip();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void EquipItem(AItemBase* item);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (AllowAbstract = "false"))
	void AddItemToInventory(TSubclassOf<AItemBase> itemClass);
};
