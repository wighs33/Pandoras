// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Interface/HudInterface.h"

#include "PandorasHud.generated.h"

class UUserWidget;
class AInventoryRoom;

UCLASS()
class PANDORAS_API APandorasHud : public AHUD, public IHudInterface
{
	GENERATED_BODY()
		
private:
	virtual void SwitchToInventory(bool b_enable, AActor* inventory_room) override;
	virtual void ToggleUIInput(bool b_input) override;
	virtual void ToggleWeaponList(bool b_enable) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InventoryMainWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> InventoryMainWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WeaponListWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> WeaponListWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AInventoryRoom> InventoryRoom;

protected:
	// hook ¿Ã∫•∆Æ
	UFUNCTION(BlueprintImplementableEvent, Category = "Hook")
	void BP_InitItems();
};
