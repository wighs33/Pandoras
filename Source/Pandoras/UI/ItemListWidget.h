// ���� �������� Ŭ������ �� ������ ���� ����Ʈ UI

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GA/GA_Equip.h"
#include "ItemListWidget.generated.h"

class UButton;
class UUniformGridPanel;
class UItemButtonWidget;
class AItemBase;
class UItemDetailsPanel;

UCLASS()
class PANDORAS_API UItemListWidget : public UUserWidget
{
	GENERATED_BODY()
	
	protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnBackButtonClicked();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void InitItems(const TArray<TSubclassOf<UGA_Equip>>& New_GA_Equip_Classes);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ChangeSelectedItem(UItemButtonWidget* NewSelectedItem);

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "C++")
	bool IsAlreadyEquipped(TSubclassOf<UGA_Equip> EquipGA);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void PeekItemDetails(UItemButtonWidget* InItemButton, bool Hovered);

	// ����
protected:
	// BindWidget: �̸��� ����θ� ������ �ڵ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemDetailsPanel> WDG_ItemDetailsPanel;

	// ����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<UItemButtonWidget*> ItemButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UItemButtonWidget> ItemButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGA_Equip>> GA_Equip_Classes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UItemButtonWidget> SelectedItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGA_Equip> Selected_GA_Equip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UDataTable> DT_ItemTable;
};
