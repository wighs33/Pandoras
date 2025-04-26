// 캐릭터의 아이템 사용 관련 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemWielderInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UItemWielderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PANDORAS_API IItemWielderInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void ForceUnequip() = 0;

	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void EquipItem(AActor* item) = 0;
};
