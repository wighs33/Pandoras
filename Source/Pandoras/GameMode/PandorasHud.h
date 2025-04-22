// HUD

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/HudInterface.h"

#include "PandorasHud.generated.h"

class UMainUIWidget;
class AInventoryRoom;

UCLASS()
class PANDORAS_API APandorasHud : public AHUD, public IHudInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
		
private:
	virtual void SwitchToInventoryUI(AActor* inventory_room) override;
	virtual void ToggleUIInput(bool b_input) override;
	virtual void SwitchToWeaponListUI() override;
	virtual void SwitchToInGameHud() override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainUIWidget> MainUIWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainUIWidget> MainUIWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AInventoryRoom> InventoryRoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> WDG_InGameHud;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> WDG_InventoryMain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> WDG_WeaponList;

protected:
	// hook ¿Ã∫•∆Æ
	UFUNCTION(BlueprintImplementableEvent, Category = "Hook")
	void BP_InitItems();

	void InitWidget();
};
