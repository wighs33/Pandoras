// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemListWidget.h"
#include "Components/Button.h"

void UItemListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ��ư Ŭ�� ���� ���ε�
	Back->OnClicked.AddDynamic(this, &UItemListWidget::OnBackButtonClicked);
}
