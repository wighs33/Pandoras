// 애니메이션 인터페이스

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
	// 무기 변수 업데이트
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateWeapon(EItem NewWeapon);

	// 이동 모드 갱신
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateMovementMode(ECustomMovementMode Mode);
};
