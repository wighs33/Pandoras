// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/BaseActorAttributes.h"
#include "Net/UnrealNetwork.h"

void UBaseActorAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ü�� ���� ���� ��� ���
	// COND_None : ���Ǿ��� �׻� ����
	// REPNOTIFY_Always: ���� ���� üũ ���� OnRep �Լ� ����
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseActorAttributes, Health, COND_None, REPNOTIFY_Always);
}
