// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemButtonWidget.h"
#include "Components/Button.h"

void UItemButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // ��ư Ŭ�� ���� ���ε�
    Button->OnClicked.AddDynamic(this, &UItemButtonWidget::OnButtonClicked);
    Button->OnHovered.AddDynamic(this, &UItemButtonWidget::OnButtonHovered);
    Button->OnUnhovered.AddDynamic(this, &UItemButtonWidget::OnButtonUnhovered);
}
