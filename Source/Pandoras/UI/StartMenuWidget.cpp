// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartMenuWidget.h"
#include "Components/Button.h"

void UStartMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 버튼 클릭 시점 바인딩
    HostButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnHostButtonClicked);
    JoinButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnJoinButtonClicked);
}