// ������ �������̽�

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IItemInterface
{
	GENERATED_BODY()
	
public:
	// �� ���� ����Ʈ ���
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ToggleSwordTrail(bool Enable);

	// ���� ����Ʈ ���
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void TriggerBlood();

	// ������ �±� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "WeaponTag"))
	FGameplayTagContainer GetItemTag();

	// ������ Ÿ�� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "WeaponType"))
	EItem GetItemType();

	// ���� ����߸���
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Drop();

	// ���� ��¡
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ChargeWeapon();

	// ������ ������ ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void GetItemData(float& DamageRadius, TSubclassOf<UGameplayEffect>& GE_Damage_Charged, TSubclassOf<UGameplayEffect>& GE_Damage_lite, TSubclassOf<UGameplayEffect>& GE_Stun);

	// ���̷�Ż �޽ø� ĳ���� ���� ���߱�
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void InitLeadPose(USkeletalMeshComponent* LeadMesh);
};
