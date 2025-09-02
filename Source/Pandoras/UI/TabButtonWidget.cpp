// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TabButtonWidget.h"
#include "Components/Button.h"

void UTabButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 버튼 클릭 시점 바인딩
    Button->OnClicked.AddDynamic(this, &UTabButtonWidget::OnButtonClicked);
}
