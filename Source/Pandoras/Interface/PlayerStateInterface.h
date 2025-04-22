// 플레이어 스테이트의 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerStateInterface.generated.h"

class AItemBase;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PANDORAS_API IPlayerStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual TArray<TSubclassOf<AItemBase>> GetItemClasses() = 0;

	UFUNCTION(BlueprintCallable, Category = "C++")
	virtual void AddItemClass(TSubclassOf<AItemBase> item) = 0;
};
