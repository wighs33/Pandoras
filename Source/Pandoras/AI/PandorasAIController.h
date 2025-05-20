// AI���� ��Ʈ�ѷ�

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
	// ������
	APandorasAIController();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Hearing;
};
