// �÷��̾� ������Ʈ�� �������̽�

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
	// ������ Ŭ���� �迭 ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	TArray<TSubclassOf<AItemBase>> GetItemClasses();

	// �迭�� ������ Ŭ���� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void AddItemClass(TSubclassOf<AItemBase> item);
};
