// AI 인터페이스

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AI_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAI_Interface : public UInterface
{
	GENERATED_BODY()
};

class PANDORAS_API IAI_Interface
{
	GENERATED_BODY()

public:
	// 타겟 갱신
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateTargetActor();

	// 적 감지 알림
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void NotifyDetectedEnemy(AActor* Enemy);
};
