// 메인 UI 위젯

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIWidget.generated.h"

class UWidgetSwitcher;
class UInGameHudWidget;
class UInventoryMainWidget;
class UItemListWidget;
class USkillTreeWidget;
class UVerticalBox;
class UTabButtonWidget;

UCLASS()
class PANDORAS_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnTabBtnMenuClicked(UTabButtonWidget* ClickedButton);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnTabBtnSkillsClicked(UTabButtonWidget* ClickedButton);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnTabBtnInventoryClicked(UTabButtonWidget* ClickedButton);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnTabBtnMapClicked(UTabButtonWidget* ClickedButton);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void HighlightTabButton(UTabButtonWidget* SelectedTabButton);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateTabs(UObject* SelectedPanel);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void SwitchWidget(UWidget* Widget);

public:
	UFUNCTION(BlueprintPure, Category = "C++")
	FORCEINLINE UInGameHudWidget* GetInGameHud() const { return WDG_InGameHud; }

	UFUNCTION(BlueprintPure, Category = "C++")
	FORCEINLINE UInventoryMainWidget* GetInventoryMainUI() const { return WDG_InventoryMain; }

	UFUNCTION(BlueprintPure, Category = "C++")
	FORCEINLINE UItemListWidget* GetItemListUI() const { return WDG_ItemList; }

	UFUNCTION(BlueprintPure, Category = "C++")
	FORCEINLINE USkillTreeWidget* GetSkillTreeUI() const { return WDG_SkillTree; }

protected:
	// BindWidget: 이름만 맞춰두면 엔진이 자동으로 연결
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInGameHudWidget> WDG_InGameHud;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryMainWidget> WDG_InventoryMain;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemListWidget> WDG_ItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USkillTreeWidget> WDG_SkillTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VBTabs;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTabButtonWidget> TabBtnMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTabButtonWidget> TabBtnSkills;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTabButtonWidget> TabBtnInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTabButtonWidget> TabBtnMap;
};
