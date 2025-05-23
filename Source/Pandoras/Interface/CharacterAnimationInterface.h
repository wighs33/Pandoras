// �ִϸ��̼� �������̽�

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterAnimationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API ICharacterAnimationInterface
{
	GENERATED_BODY()

public:
	// ���� ���� ������Ʈ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateWeapon(EItem NewWeapon);

	// �̵� ��� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateMovementMode(ECustomMovementMode Mode);
};
