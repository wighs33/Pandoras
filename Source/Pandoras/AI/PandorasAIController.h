// AI���� ��Ʈ�ѷ�

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PandorasAIController.generated.h"

class UAIPerceptionComponent;

UCLASS()
class PANDORAS_API APandorasAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// ������
	APandorasAIController();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Hearing;
};
