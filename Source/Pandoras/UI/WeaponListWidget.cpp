// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponListWidget.h"
#include "Components/Button.h"

void UWeaponListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 시점 바인딩
	Back->OnClicked.AddDynamic(this, &UWeaponListWidget::OnBackButtonClicked);
}
