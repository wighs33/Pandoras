// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemListWidget.h"
#include "Components/Button.h"

void UItemListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 시점 바인딩
	Back->OnClicked.AddDynamic(this, &UItemListWidget::OnBackButtonClicked);
}
