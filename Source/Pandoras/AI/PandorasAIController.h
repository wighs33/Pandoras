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
	// 타겟 갱신
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "ClosestActor"))
	AActor* GetClosestDetectedActor();

// 컴포넌트
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Hearing;

// 변수
protected:
	// 현재 타겟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> CurrentTarget;

	// 감지된 모든 타겟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<AActor*> DetectedTargets;
};
