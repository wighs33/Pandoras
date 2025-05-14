// 파벌 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FactionsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFactionsInterface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IFactionsInterface
{
	GENERATED_BODY()

public:
	// 파벌 아이디 반환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "Faction"))
	int32 GetFactionId();
};
