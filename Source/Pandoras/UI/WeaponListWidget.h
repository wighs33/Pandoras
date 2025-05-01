// ���� �������� Ŭ������ �� ������ ���� ����Ʈ UI

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponListWidget.generated.h"

class UButton;
class UUniformGridPanel;
class UItemButtonWidget;
class AItemBase;

UCLASS()
class PANDORAS_API UWeaponListWidget : public UUserWidget
{
	GENERATED_BODY()
	
	protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnBackButtonClicked();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void InitItems(const TArray<TSubclassOf<AItemBase>>& item_classes);

	// ����
protected:
	// BindWidget: �̸��� ����θ� ������ �ڵ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemPanel;

	// ����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<UItemButtonWidget*> ItemButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UItemButtonWidget> ItemButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<AItemBase>> ItemClasses;
};
