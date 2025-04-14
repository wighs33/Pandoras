// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PandorasHud.h"
#include "Blueprint/UserWidget.h"
#include "Room/InventoryRoom.h"

void APandorasHud::SwitchToInventory(bool b_enable, AActor* inventory_room)
{
	if (!InventoryRoom)
	{
		if (inventory_room)
		{
			InventoryRoom = CastChecked<AInventoryRoom>(inventory_room);
		}
	}


	if (b_enable)
	{
		if (!InventoryMainWidget)
		{
			APlayerController* pc = GetOwningPlayerController();
			check(pc);

			InventoryMainWidget = CreateWidget<UUserWidget>(pc, InventoryMainWidgetClass);
			check(InventoryMainWidget);
		}
		InventoryMainWidget->AddToViewport();
		InventoryRoom->ChangeFocusPoint(ECharacterFocusPoint::Main);
	}
	else
	{
		check(InventoryMainWidget);

		InventoryMainWidget->RemoveFromParent();
	}
}

void APandorasHud::ToggleUIInput(bool b_input)
{
	APlayerController* pc = GetOwningPlayerController();
	check(pc);

	if (b_input)
	{
		pc->SetInputMode(FInputModeGameAndUI());
		pc->bShowMouseCursor = true;
	}
	else
	{
		pc->SetInputMode(FInputModeGameOnly());
		pc->bShowMouseCursor = false;
	}
}

void APandorasHud::ToggleWeaponList(bool b_enable)
{
	if (b_enable)
	{
		if (!WeaponListWidget)
		{
			APlayerController* pc = GetOwningPlayerController();
			check(pc);

			WeaponListWidget = CreateWidget<UUserWidget>(pc, WeaponListWidgetClass);
			check(WeaponListWidget);
		}
		WeaponListWidget->AddToViewport();
		SwitchToInventory(false, nullptr);

		check(InventoryRoom);
		InventoryRoom->ChangeFocusPoint(ECharacterFocusPoint::Weapon);
	}
	else
	{
		check(WeaponListWidget);

		WeaponListWidget->RemoveFromParent();
		SwitchToInventory(true, nullptr);
	}
}
