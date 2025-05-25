// 위치 주변 탐색 태스크

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SearchAroundLocation.generated.h"

DECLARE_DELEGATE(FAICharacterAttackFinished);

UCLASS()
class PANDORAS_API UBTTask_SearchAroundLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SearchAroundLocation()
	{
		NodeName = TEXT("Search Around Location");

		// 이 Task는 비동기가 아니므로 바로 끝냄
		bNotifyTick = false;
	}

	/** 블랙보드에 저장된 원점 키 */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector SearchLocationKey;

	/** 반경 */
	float Radius = 1000.f;

	/** AI가 목적지에 얼마나 가까이 가면 성공으로 간주할지 */
	float AcceptanceRadius = 5.f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
};
