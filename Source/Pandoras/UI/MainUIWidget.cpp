// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUIWidget.h"
#include "Components/WidgetSwitcher.h"

//void UMainUIWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//}

void UMainUIWidget::SwitchWidget(UUserWidget* wdg)
{
	if (WidgetSwitcher && wdg)
	{
		WidgetSwitcher->SetActiveWidget(wdg);
	}
}
