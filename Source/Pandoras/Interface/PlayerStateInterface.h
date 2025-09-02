// 플레이어 스테이트의 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerStateInterface.generated.h"

class UGA_Equip;

UINTERFACE(Blueprintable, BlueprintType)
class UPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IPlayerStateInterface
{
	GENERATED_BODY()

public:
	// 타입 별로 아아템 클래스 배열 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	TArray<TSubclassOf<UGA_Equip>> GetItemAbilityClassesByType(EItem ItemType);

	// 배열에 아이템 클래스 저장
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void AddItemClass(TSubclassOf<UGA_Equip> itemAbility);

	// 아이템들 한곳에 모아서 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	TArray<TSubclassOf<UGA_Equip>> GetAllItemAbilities();
};
