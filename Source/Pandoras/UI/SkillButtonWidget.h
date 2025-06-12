// ��ų ��ư ����

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/Structs.h"
#include "SkillButtonWidget.generated.h"

class UButton;
class UImage;
class UBorder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillButtonClicked, int32, Index);

UCLASS()
class PANDORAS_API USkillButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnClickDispatch(int32 Index) { OnClickDispatch.Broadcast(Index); }

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
	FSkill SkillData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	bool SkillActivated;

// ��������Ʈ
protected:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnSkillButtonClicked OnClickDispatch;
};
