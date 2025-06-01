// 캐릭터의 아이템 사용 관련 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Common/Enums.h"
#include "ItemWielderInterface.generated.h"

class UGA_Equip;

UINTERFACE(Blueprintable, BlueprintType)
class UItemWielderInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IItemWielderInterface
{
	GENERATED_BODY()

public:
	// 아이템 장착
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void EquipItem(AItemBase* item);

	// 아이템 장착 해제
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void UnequipItem();

	// 아이템을 플레이어 스테이트에 저장
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (AllowAbstract = "false"))
	void AddItemToPlayerState(TSubclassOf<UGA_Equip> itemAbilityClass);

	// 아이템 파괴
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")	
	void DestroyItem(EItem ItemType);

	// 무기 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "Weapon"))
	AItemBase* GetWeapon();

	// 공격 중 검 위치 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "AttackState"))
	EAttackState GetAttackState();

	// 공격 중 검 위치 설정
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void SetAttackState(EAttackState AttackState);

	// 공격 예고 알림
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void NotifyAttack(bool IsNonBlockable);

	// 차징 공격
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ChargeAttack();
};
