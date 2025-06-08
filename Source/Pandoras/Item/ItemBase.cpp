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

// 해당 메시의 스켈레톤에 딱맞게 부착
void AItemBase::InitLeadPose_Implementation(USkeletalMeshComponent* LeadMesh)
{
	// 두 컴포넌트의 유효성 체크
	if (!SK_Item || !LeadMesh)
	{
		return;
	}

	// 1) SK_Item 의 로컬 위치를 (0,0,0) 으로 리셋
	SK_Item->SetRelativeLocation(FVector::ZeroVector);

	// 2) SK_Item 에 새 LeaderPoseComponent 설정
	SK_Item->SetLeaderPoseComponent(LeadMesh, /*bForceUpdate=*/false, /*bInFollowerShouldTickPose=*/false);
}

// 아이템 데이터들 반환
void AItemBase::GetItemData_Implementation(float& Out_DamageRadius, TSubclassOf<UGameplayEffect>& Out_GE_Damage_Charged, TSubclassOf<UGameplayEffect>& Out_GE_Damage_lite, TSubclassOf<UGameplayEffect>& Out_GE_Stun)
{
	Out_DamageRadius = DamageRadius;
	Out_GE_Damage_Charged = GE_Damage_Charged;
	Out_GE_Damage_lite = GE_Damage_lite;
	Out_GE_Stun = GE_Stun;
}
