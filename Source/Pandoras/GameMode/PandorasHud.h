// HUD

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/HudInterface.h"
#include "Item/ItemBase.h"

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
	// 인터페이스 함수들 C++ 구현
	virtual void SwitchToInventoryUI_Implementation(AActor* InventoryRoom) override;
	virtual void ToggleUIInput_Implementation(bool bInput) override;
	virtual void SwitchToWeaponListUI_Implementation() override;
	virtual void SwitchToInGameHud_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent, Category="C++")
	void ShowRightNotification(const FNotification& NotificationData);

	UFUNCTION(BlueprintImplementableEvent, Category = "C++")
	void BP_InitItems(const TArray<TSubclassOf<AItemBase>>& ItemClasses);

	void InitWidget();
};
