// ĳ������ ������ ��� ���� �������̽�

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
	// ������ ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void EquipItem(AItemBase* item);

	// ������ ���� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void UnequipItem();

	// �������� �÷��̾� ������Ʈ�� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (AllowAbstract = "false"))
	void AddItemToPlayerState(TSubclassOf<UGA_Equip> itemAbilityClass);

	// ������ �ı�
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")	
	void DestroyItem(EItem ItemType);

	// ���� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "Weapon"))
	AItemBase* GetWeapon();

	// ���� �� �� ��ġ ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "AttackState"))
	EAttackState GetAttackState();

	// ���� �� �� ��ġ ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void SetAttackState(EAttackState AttackState);

	// ���� ���� �˸�
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void NotifyAttack(bool IsNonBlockable);

	// ��¡ ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++")
	void ChargeAttack();
};
