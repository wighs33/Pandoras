// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDetailsPanel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivateItemButtonClick);

UCLASS()
class PANDORAS_API UItemDetailsPanel : public UUserWidget
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnActivateButtonClick() { OnActivateButtonClick.Broadcast(); }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void InitializeData(UItemButtonWidget* ItemButton);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Hide();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void InitializeStats();

// ����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	bool ClickedShow;
	
// ��������Ʈ
protected:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnActivateItemButtonClick OnActivateButtonClick;
};
