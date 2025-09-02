// 판도라즈의 애니메이션 관리

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Common/Enums.h"
#include "Interface/CharacterAnimationInterface.h"

#include "PandorasAnimInstance.generated.h"

// 애니메이션 인터페이스 사용
UCLASS()
class PANDORAS_API UPandorasAnimInstance : public UAnimInstance, public ICharacterAnimationInterface
{
	GENERATED_BODY()

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	EItem Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	ECustomMovementMode CurrentMovementMode = ECustomMovementMode::Walk;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	float CrouchAlpha;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	float CrouchDelayedSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	bool Focus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	FVector LookAtLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> LockedTarget;
};
