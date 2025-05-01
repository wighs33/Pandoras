// 플레이어 스테이트의 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerStateInterface.generated.h"

class AItemBase;

UINTERFACE(Blueprintable, BlueprintType)
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

public:
	// 아이템 클래스 배열 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	TArray<TSubclassOf<AItemBase>> GetItemClasses();

	// 배열에 아이템 클래스 저장
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void AddItemClass(TSubclassOf<AItemBase> item);
};
