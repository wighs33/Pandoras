// HUD의 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Common/Structs.h"
#include "Common/Enums.h"
#include "HudInterface.generated.h"

class AInventoryRoom;

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UHudInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IHudInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void SwitchToInventoryUI(AInventoryRoom* inventory_room);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ToggleUIInput(bool bInput);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void SwitchToItemListUI(EItem ItemType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void SwitchToInGameHud();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ShowRightNotification(FNotification NotificationData);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void UpdateHealth(float Health);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void UpdateStamina(float Stamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ShowTip(const FString& TipText, float DisplayTime = 2.0);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ShowCharacterLevel(float ShowDuration = 10.0f);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void SwitchToSkillMenu();
};
