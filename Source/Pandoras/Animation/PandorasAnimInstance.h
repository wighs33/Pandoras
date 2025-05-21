// �ǵ������� �ִϸ��̼� ����

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Common/Enums.h"
#include "Interface/CharacterAnimationInterface.h"

#include "PandorasAnimInstance.generated.h"

// �ִϸ��̼� �������̽� ���
UCLASS()
class PANDORAS_API UPandorasAnimInstance : public UAnimInstance, public ICharacterAnimationInterface
{
	GENERATED_BODY()

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	EItem Weapon;
};
