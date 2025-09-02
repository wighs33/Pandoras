// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeLinesWidget.generated.h"

class USkillButtonWidget;

UCLASS()
class PANDORAS_API USkillTreeLinesWidget : public UUserWidget
{
	GENERATED_BODY()

// º¯¼ö
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TObjectPtr<USkillButtonWidget>> LineStartButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TObjectPtr<USkillButtonWidget>> LineEndButtons;
};
