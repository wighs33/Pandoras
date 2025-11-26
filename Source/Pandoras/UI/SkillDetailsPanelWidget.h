#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/Structs.h"
#include "SkillDetailsPanelWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivateSkillButtonClick);

class UTextBlock;
class UImage;
class UFillButtonWidget;
class UButton;

UCLASS()
class PANDORAS_API USkillDetailsPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnActivateButtonClick() { OnActivateButtonClick.Broadcast(); }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void InitializeData(USkillButtonWidget* SkillButton);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void Hide();

	UFUNCTION()
	void Activate();

	UFUNCTION()
	void HandleCancelClicked();

	FORCEINLINE bool GetClickedShow() { return ClickedShow; }
	FORCEINLINE void SetClickedShow(bool flag) { ClickedShow = flag; }

// 위젯
protected:
	// BindWidget: 이름만 맞춰두면 엔진이 자동으로 연결
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextSkillName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextSkillDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> TextSkillCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ImgSkill;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
    FSkill SkillData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UFillButtonWidget> FillButtonActivate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> BCancel;

// 변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	bool ClickedShow;
	
// 델리게이트
public:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnActivateSkillButtonClick OnActivateButtonClick;
};
