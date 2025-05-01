// ¿Œ∞‘¿” HUD

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameHudWidget.generated.h"

class URightNotificationWidget;

UCLASS()
class PANDORAS_API UInGameHudWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URightNotificationWidget> WDG_RightNotification;
};
