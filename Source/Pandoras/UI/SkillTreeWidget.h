// ��ų Ʈ�� ����

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class UVerticalBox;
class USkillButtonWidget;

UCLASS()
class PANDORAS_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void InitializeTree(UDataTable* InDTSkills, UVerticalBox* InVerticalBox);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateSkillStatus(USkillButtonWidget* SkillButton);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void RefreshSkillTree();

// ����
protected:
	// BindWidget: �̸��� ����θ� ������ �ڵ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VBMain;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VBSkillTree1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VBSkillTree2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VBSkillTree3;

// ����
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UDataTable> DTCombatSkills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UDataTable> DTSurvivalSkills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UDataTable> DTMagicSkills;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TMap<FName, TObjectPtr<USkillButtonWidget>> SkillButtonMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TObjectPtr<USkillButtonWidget>> LineStartButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TObjectPtr<USkillButtonWidget>> LineEndButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	bool MouseDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	FVector2D CachedMousePosition;
};
