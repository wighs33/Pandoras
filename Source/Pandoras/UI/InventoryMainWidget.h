// 인벤토리 메인

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMainWidget.generated.h"

class UItemButtonWidget;
class ULevelWidget;

UCLASS()
class PANDORAS_API UInventoryMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
// 위젯
protected:
	// BindWidget: 이름만 맞춰두면 엔진이 자동으로 연결
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemButtonWidget> BTN_Helmet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemButtonWidget> BTN_Armor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemButtonWidget> BTN_Shoes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemButtonWidget> BTN_Glove;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemButtonWidget> BTN_Sword;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemButtonWidget> BTN_Bow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<ULevelWidget> WDG_Level;
};
