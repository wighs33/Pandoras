// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabButtonWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTabButtonClicked, UTabButtonWidget*, ClickedButton);

UCLASS()
class PANDORAS_API UTabButtonWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnTabButtonClicked(UTabButtonWidget* ClickedButton) { OnTabButtonClicked.Broadcast(ClickedButton); }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnButtonClicked();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Highlight(bool Enable);

// 위젯
protected:
	// BindWidget: 이름만 맞춰두면 엔진이 자동으로 연결
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonText;

// 변수
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FText ButtonName;

// 델리게이트
public:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnTabButtonClicked OnTabButtonClicked;
};
