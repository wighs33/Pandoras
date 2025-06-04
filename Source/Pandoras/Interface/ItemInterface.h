// 아이템 인터페이스

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
	// 검 궤적 이펙트 토글
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ToggleSwordTrail(bool Enable);

	// 출혈 이펙트 재생
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void TriggerBlood();

	// 아이템 태그 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "WeaponTag"))
	FGameplayTagContainer GetItemTag();

	// 아이템 타입 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "WeaponType"))
	EItem GetItemType();

	// 무기 떨어뜨리기
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Drop();

	// 무기 차징
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ChargeWeapon();

	// 아이템 데이터 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void GetItemData(float& DamageRadius, TSubclassOf<UGameplayEffect>& GE_Damage_Charged, TSubclassOf<UGameplayEffect>& GE_Damage_lite, TSubclassOf<UGameplayEffect>& GE_Stun);

	// 스켈레탈 메시를 캐릭터 본에 맞추기
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void InitLeadPose(USkeletalMeshComponent* LeadMesh);
};
