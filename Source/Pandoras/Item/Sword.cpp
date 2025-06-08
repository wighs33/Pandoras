// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Sword.h"
#include "Net/UnrealNetwork.h"

// �� Ŭ������ ��Ʈ��ũ�� �ö󰡴� ������ ȣ��
void ASword::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ItemClasses ������ ���� ��Ͽ� �߰�
	DOREPLIFETIME(ASword, TrailEnabled);
	DOREPLIFETIME(ASword, BloodTriggered);
}
