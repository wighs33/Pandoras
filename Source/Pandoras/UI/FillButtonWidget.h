#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FillButtonWidget.generated.h"

class UButton;
class UBorder;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonFilled);

UCLASS()
class PANDORAS_API UFillButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	void CallOnFilled() { OnFilled.Broadcast(); }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Reset();

	UFUNCTION()
	void SetButtonEnabled(bool flag);
	
	// 위젯
protected:
	// BindWidget: 이름만 맞춰두면 엔진이 자동으로 연결
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> FillBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Text;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FText InText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool Holding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Percent = 0.0f;

// 델리게이트
public:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnButtonFilled OnFilled;

};
