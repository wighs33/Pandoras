// �Ĺ� �������̽�

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
	// �Ĺ� ���̵� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="C++", meta = (ReturnDisplayName = "Faction"))
	int32 GetFactionId();
};
