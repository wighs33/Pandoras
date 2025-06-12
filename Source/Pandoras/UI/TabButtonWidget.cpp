// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TabButtonWidget.h"
#include "Components/Button.h"

void UTabButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // ��ư Ŭ�� ���� ���ε�
    Button->OnClicked.AddDynamic(this, &UTabButtonWidget::OnButtonClicked);
}
