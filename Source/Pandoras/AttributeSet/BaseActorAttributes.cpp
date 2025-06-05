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
			// ���� ��ġ�� ���� ������ �� ����
			Delta = Delta * (200 - Def) / 200;
			UE_LOG(LogTemp, Warning, TEXT("Defence : %f"), Def);
			NewValue = Delta + Health.GetCurrentValue();
		}
		//NewValue = FMath::Clamp(NewValue, 0.f, MaxHealth.GetCurrentValue());
	}
}