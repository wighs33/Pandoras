// ������ ��ư

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/Structs.h"
#include "ItemButtonWidget.generated.h"

class UButton;
class UImage;
class UBorder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemButtonClicked, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemButtonHovered, UItemButtonWidget*, ItemButton, bool, Hovered);

UCLASS()
class PANDORAS_API UItemButtonWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnButtonClicked();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnButtonHovered();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnButtonUnhovered();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ChangeButtonImage(UTexture2D* Image);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ToggleSelect(bool bSelected);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void InitializeItemData(FItemData InItemData);

	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnClickDispatch(int32 Index) { OnClickDispatch.Broadcast(Index); }

	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnHoverDispatch(UItemButtonWidget* ItemButton, bool Hovered) { OnHoverDispatch.Broadcast(ItemButton, Hovered); }

// ����
protected:
	// BindWidget: �̸��� ����θ� ������ �ڵ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BorderImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> Border;

// ����
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UTexture2D> ButtonImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	int32 ButtonIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	bool Selected;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnItemButtonClicked OnClickDispatch;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnItemButtonHovered OnHoverDispatch;
};
