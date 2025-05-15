// 아이템 인터페이스

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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ToggleSwordTrail(bool Enable);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void TriggerBlood();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "WeaponTag"))
	FGameplayTagContainer GetItemTag();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "WeaponType"))
	EItem GetItemType();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Drop();
};
