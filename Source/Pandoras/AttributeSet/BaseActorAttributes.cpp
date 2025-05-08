// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/BaseActorAttributes.h"
#include "Net/UnrealNetwork.h"

void UBaseActorAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 체력 값을 복제 대상에 등록
	// COND_None : 조건없이 항상 복제
	// REPNOTIFY_Always: 변경 여부 체크 없이 OnRep 함수 실행
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseActorAttributes, Health, COND_None, REPNOTIFY_Always);
}
