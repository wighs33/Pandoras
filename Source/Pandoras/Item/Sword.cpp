// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Sword.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

ASword::ASword()
	: AItemBase()
{
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	BlockBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockBox"));

	HitBox->SetupAttachment(SK_Item);
	BlockBox->SetupAttachment(SK_Item);
}

void ASword::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Ʈ���� ���ε�
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnHitBoxBeginOverlap);
	BlockBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnBlockBoxBeginOverlap);
}


// �� Ŭ������ ��Ʈ��ũ�� �ö󰡴� ������ ȣ��
void ASword::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ItemClasses ������ ���� ��Ͽ� �߰�
	DOREPLIFETIME(ASword, TrailEnabled);
	DOREPLIFETIME(ASword, BloodTriggered);
}
