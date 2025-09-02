// 알람 영역 UI

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/Structs.h"
#include "RightNotificationWidget.generated.h"

class UVerticalBox;
class UNotificationIconWidget;

UCLASS()
class PANDORAS_API URightNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void DequeNotification();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void EnqueueNotification(FNotification NotificationData);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void RemoveNotification();

protected:
	// BindWidget: 이름만 맞춰두면 엔진이 자동으로 연결
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNotificationIconWidget> WDG_NotificationIcon_0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNotificationIconWidget> WDG_NotificationIcon_1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNotificationIconWidget> WDG_NotificationIcon_2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNotificationIconWidget> WDG_NotificationIcon_3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UNotificationIconWidget> WDG_NotificationIcon_4;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<UNotificationIconWidget*> AvailableNotificationWidgets;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<UNotificationIconWidget*> InUseNotificationWidgets;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UNotificationIconWidget> TempNotificationWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<FNotification> NotificationQueue;
};
