// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"

// Sets default values
AItemBase::AItemBase()
{
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SK_Item = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Item"));

    RootComponent = DefaultSceneRoot;
    SK_Item->SetupAttachment(DefaultSceneRoot);
}

// �ش� �޽��� ���̷��濡 ���°� ����
void AItemBase::InitLeadPose_Implementation(USkeletalMeshComponent* LeadMesh)
{
	// �� ������Ʈ�� ��ȿ�� üũ
	if (!SK_Item || !LeadMesh)
	{
		return;
	}

	// 1) SK_Item �� ���� ��ġ�� (0,0,0) ���� ����
	SK_Item->SetRelativeLocation(FVector::ZeroVector);

	// 2) SK_Item �� �� LeaderPoseComponent ����
	SK_Item->SetLeaderPoseComponent(LeadMesh, /*bForceUpdate=*/false, /*bInFollowerShouldTickPose=*/false);
}

// ������ �����͵� ��ȯ
void AItemBase::GetItemData_Implementation(float& Out_DamageRadius, TSubclassOf<UGameplayEffect>& Out_GE_Damage_Charged, TSubclassOf<UGameplayEffect>& Out_GE_Damage_lite, TSubclassOf<UGameplayEffect>& Out_GE_Stun)
{
	Out_DamageRadius = DamageRadius;
	Out_GE_Damage_Charged = GE_Damage_Charged;
	Out_GE_Damage_lite = GE_Damage_lite;
	Out_GE_Stun = GE_Stun;
}
