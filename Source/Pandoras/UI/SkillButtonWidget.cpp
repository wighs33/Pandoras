// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillButtonWidget.h"
#include "Components/Button.h"

void USkillButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 버튼 클릭 시점 바인딩
    Button->OnClicked.AddDynamic(this, &USkillButtonWidget::OnButtonClicked);
    Button->OnHovered.AddDynamic(this, &USkillButtonWidget::OnButtonHovered);
    Button->OnUnhovered.AddDynamic(this, &USkillButtonWidget::OnButtonUnhovered);
}
