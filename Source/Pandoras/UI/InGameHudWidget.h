// 인게임 HUD

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHudWidget.generated.h"

class URightNotificationWidget;
class UProgressBar;
class UImage;

UCLASS()
class PANDORAS_API UInGameHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateHealth(float InPercent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateStamina(float InPercent);
	
// 위젯
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressHealthRed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Crest;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URightNotificationWidget> WDG_RightNotification;

// 변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	float TargetHealthPercent = 1.0;
};
