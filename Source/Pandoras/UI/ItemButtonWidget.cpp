// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemButtonWidget.h"
#include "Components/Button.h"

void UItemButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 버튼 클릭 시점 바인딩
    Button->OnClicked.AddUniqueDynamic(this, &UItemButtonWidget::OnButtonClicked);
    Button->OnHovered.AddUniqueDynamic(this, &UItemButtonWidget::OnButtonHovered);
    Button->OnUnhovered.AddUniqueDynamic(this, &UItemButtonWidget::OnButtonUnhovered);
}
