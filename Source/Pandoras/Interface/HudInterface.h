// HUD의 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HudInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UHudInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IHudInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void SwitchToInventoryUI(AActor* inventory_room) = 0;

	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void ToggleUIInput(bool bInput) = 0;

	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void SwitchToWeaponListUI() = 0;

	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void SwitchToInGameHud() = 0;
};
