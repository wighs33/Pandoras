// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponListWidget.h"
#include "Components/Button.h"

void UWeaponListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ��ư Ŭ�� ���� ���ε�
	Back->OnClicked.AddDynamic(this, &UWeaponListWidget::OnBackButtonClicked);
}
