// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillDetailsPanelWidget.generated.h"

class USkillDetailsPanelWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivateButtonClick);

UCLASS()
class PANDORAS_API USkillDetailsPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnActivateButtonClick() { OnActivateButtonClick.Broadcast(); }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void InitializeData(USkillButtonWidget* SkillButton);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Hide();

// 변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	bool ClickedShow;
	
// 델리게이트
protected:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnActivateButtonClick OnActivateButtonClick;
};
