// AI전용 컨트롤러

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "PandorasAIController.generated.h"

class UAIPerceptionComponent;

UCLASS()
class PANDORAS_API APandorasAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()
	
public:
	// 생성자
	APandorasAIController();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Hearing;
};
