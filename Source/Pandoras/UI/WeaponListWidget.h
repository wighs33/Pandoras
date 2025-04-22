// ���� �������� Ŭ������ �� ������ ���� ����Ʈ UI

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
	// BindWidget: �̸��� ����θ� ������ �ڵ����� ����
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Back;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemPanel;

//private:
//	// �������Ʈ���� Ŭ���� ����
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
//	TSubclassOf<UEquipmentButtonWidget> EquipmentButtonClass;
//
//	// ������ ��ư�� ����
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
//	TArray<UEquipmentButtonWidget*> ItemButtons;

//protected:
//
//	virtual void NativeConstruct() override;
//
//private:
//	void InitializeItems();
};
