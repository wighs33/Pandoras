// ���� UI ����

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIWidget.generated.h"

class UWidgetSwitcher;

UCLASS()
class PANDORAS_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()

//protected:
//	virtual void NativeConstruct() override;

protected:
	// BindWidget: �̸��� ����θ� ������ �ڵ����� ����
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> WDG_InGameHud;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> WDG_InventoryMain;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> WDG_WeaponList;

public:
	FORCEINLINE UUserWidget* GetInGameHud() { return WDG_InGameHud; }
	FORCEINLINE UUserWidget* GetInventoryMain() { return WDG_InventoryMain; }
	FORCEINLINE UUserWidget* GetWeaponList() { return WDG_WeaponList; }

	UFUNCTION()
	void SwitchWidget(UUserWidget* wdg);
};
