// HUD

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/HudInterface.h"
#include "Item/ItemBase.h"

#include "PandorasHud.generated.h"

class UMainUIWidget;
class AInventoryRoom;
class UInGameHudWidget;
class UInventoryMainWidget;
class UItemListWidget;
class ULevelWidget;
class USkillTreeWidget;

UCLASS()
class PANDORAS_API APandorasHud : public AHUD, public IHudInterface
{
	GENERATED_BODY()

//protected:
//	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainUIWidget> MainUIWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainUIWidget> MainUIWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AInventoryRoom> InventoryRoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInGameHudWidget> WDG_InGameHud;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryMainWidget> WDG_InventoryMain;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UItemListWidget> WDG_ItemList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULevelWidget> WDG_CharacterLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkillTreeWidget> WDG_SkillTree;

protected:
	//// 인터페이스 함수들 C++ 구현
	//virtual void SwitchToInventoryUI_Implementation(AActor* InventoryRoom) override;
	//virtual void ToggleUIInput_Implementation(bool bInput) override;
	//virtual void SwitchToWeaponListUI_Implementation() override;
	//virtual void SwitchToInGameHud_Implementation() override;

	//UFUNCTION(BlueprintImplementableEvent, Category="C++")
	//void ShowRightNotification(const FNotification& NotificationData);

	//void InitWidget();
};
