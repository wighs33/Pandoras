// ���� �˸� ������

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationIconWidget.generated.h"

class UTextBlock;
class UItemButtonWidget;

UCLASS()
class PANDORAS_API UNotificationIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateNotification(const FText& InText, UTexture2D* InIcon);

protected:
	// BindWidget: �̸��� ����θ� ������ �ڵ����� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Text;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemButtonWidget> WDG_ItemButton;
};
