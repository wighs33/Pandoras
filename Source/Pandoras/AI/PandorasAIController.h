// AI전용 컨트롤러

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "PandorasAIController.generated.h"

class UAIPerceptionComponent;
class ALastSeen;

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

	// 마지막 스냅샷 보여주기
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "ClosestActor"))
	void ToggleLastSnapshot(AActor* TargetActor, bool ShowOrHide);

	// 가까이 있는 npc들에게 경고
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "ClosestActor"))
	void AlertNearbyNPCs(AActor* DetectedEnemy);

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

	// 마지막으로 본 위치 스냅샷
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<ALastSeen> LastSeenSnapshot;
};
