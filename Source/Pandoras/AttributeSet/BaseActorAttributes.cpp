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
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseActorAttributes, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseActorAttributes, Defence, COND_None, REPNOTIFY_Always);
}

void UBaseActorAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		float Delta = NewValue - Health.GetCurrentValue();
		if (Delta < 0)
		{
			float Def = FMath::Clamp(Defence.GetCurrentValue(), 0.f, 100.f);
			// 방어력 수치에 따라 데미지 량 감소
			Delta = Delta * (200 - Def) / 200;
			UE_LOG(LogTemp, Warning, TEXT("Defence : %f"), Def);
			NewValue = Delta + Health.GetCurrentValue();
		}
		//NewValue = FMath::Clamp(NewValue, 0.f, MaxHealth.GetCurrentValue());
	}
}