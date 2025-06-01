// �����Ƽ �ý��� �������̽�

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GASInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGASInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IGASInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void GiveAndActivateAbility_Replicate(TSubclassOf<UGameplayAbility> Ability);
};
