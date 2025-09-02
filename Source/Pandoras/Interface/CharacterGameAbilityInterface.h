// 캐릭터의 어빌리티 관련 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterGameAbilityInterface.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class UCharacterGameAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PANDORAS_API ICharacterGameAbilityInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void SendGameplayEvent_Replicated(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ExecuteGameplayCue_Replicated(AActor* TargetActor, FGameplayTag GamplayCueTag, FGameplayCueParameters Parameters);
};
