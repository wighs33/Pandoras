// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartMenuWidget.h"
#include "Components/Button.h"

void UStartMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // ��ư Ŭ�� ���� ���ε�
    HostButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnHostButtonClicked);
    JoinButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnJoinButtonClicked);
}