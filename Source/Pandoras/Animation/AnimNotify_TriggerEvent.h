// Ʈ���� �̺�Ʈ ��Ƽ����

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_TriggerEvent.generated.h"

UCLASS()
class PANDORAS_API UAnimNotify_TriggerEvent : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayTag EventTag;
};
