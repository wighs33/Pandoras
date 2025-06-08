// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Sword.h"
#include "Net/UnrealNetwork.h"

// 이 클래스가 네트워크에 올라가는 시점에 호출
void ASword::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ItemClasses 변수를 복제 목록에 추가
	DOREPLIFETIME(ASword, TrailEnabled);
	DOREPLIFETIME(ASword, BloodTriggered);
}
