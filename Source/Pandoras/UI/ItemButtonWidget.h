// ������ ��ư

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemButtonWidget.generated.h"

class UButton;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, int32, Index);

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

	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnClickDispatch(int32 Index) { OnClickDispatch.Broadcast(Index); }

// ����
protected:
	// BindWidget: �̸��� ����θ� ������ �ڵ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BorderImage;

// ����
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UTexture2D> ButtonImage;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	int32 ButtonIndex;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnButtonClicked OnClickDispatch;
};
