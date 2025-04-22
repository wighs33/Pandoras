// 무기 아이콘을 클릭했을 때 나오는 무기 리스트 UI

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponListWidget.generated.h"

class UButton;
class UUniformGridPanel;

UCLASS()
class PANDORAS_API UWeaponListWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// BindWidget: 이름만 맞춰두면 엔진이 자동으로 연결
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Back;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemPanel;

//private:
//	// 블루프린트에서 클래스 지정
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
//	TSubclassOf<UEquipmentButtonWidget> EquipmentButtonClass;
//
//	// 생성된 버튼을 보관
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
//	TArray<UEquipmentButtonWidget*> ItemButtons;

//protected:
//
//	virtual void NativeConstruct() override;
//
//private:
//	void InitializeItems();
};
