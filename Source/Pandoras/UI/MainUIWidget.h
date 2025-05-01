// ���� UI ����

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIWidget.generated.h"

class UWidgetSwitcher;
class UInGameHudWidget;
class UInventoryMainWidget;
class UWeaponListWidget;

UCLASS()
class PANDORAS_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()

//protected:
//	virtual void NativeConstruct() override;

protected:
	// BindWidget: �̸��� ����θ� ������ �ڵ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInGameHudWidget> WDG_InGameHud;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryMainWidget> WDG_InventoryMain;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWeaponListWidget> WDG_WeaponList;

public:
	UFUNCTION(BlueprintPure, Category = "C++")
	FORCEINLINE UInGameHudWidget* GetInGameHud() const { return WDG_InGameHud; }

	UFUNCTION(BlueprintPure, Category = "C++")
	FORCEINLINE UInventoryMainWidget* GetInventoryMainUI() const { return WDG_InventoryMain; }

	UFUNCTION(BlueprintPure, Category = "C++")
	FORCEINLINE UWeaponListWidget* GetWeaponListUI() const { return WDG_WeaponList; }

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void SwitchWidget(UUserWidget* wdg);
};
