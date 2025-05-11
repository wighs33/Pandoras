// 캐릭터의 아이템 사용 관련 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Common/Enums.h"
#include "ItemWielderInterface.generated.h"

class AItemBase;

UINTERFACE(Blueprintable, BlueprintType)
class UItemWielderInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IItemWielderInterface
{
	GENERATED_BODY()

public:
	// 아이템 장착
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void EquipItem(AItemBase* item);

	// 아이템 장착 해제
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void UnequipItem();

	// 아이템을 인벤토리로 추가
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (AllowAbstract = "false"))
	void AddItemToInventory(TSubclassOf<AItemBase> itemClass);

	// 아이템 파괴
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void DestroyItem(EItem ItemType);

	// 아이템 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "Weapon"))
	AItemBase* GetWeapon();
};
