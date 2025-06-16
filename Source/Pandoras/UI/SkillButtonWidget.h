// 스킬 버튼 위젯

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/Structs.h"
#include "SkillButtonWidget.generated.h"

class UButton;
class UImage;
class UBorder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillButtonClicked);

UCLASS()
class PANDORAS_API USkillButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnClickDispatch() { OnClickDispatch.Broadcast(); }

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
	void MarkAsActivated();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ToggleSkillLocked(bool Lock);

// 위젯
protected:
	// BindWidget: 이름만 맞춰두면 엔진이 자동으로 연결
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> BorderImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> Border;

// 변수
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UTexture2D> ButtonImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	int32 ButtonIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	bool Selected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (ExposeOnSpawn = "true"))
	FSkill SkillData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	bool SkillActivated;

// 델리게이트
protected:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnSkillButtonClicked OnClickDispatch;
};
