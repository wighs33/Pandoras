// 위치 주변 탐색 태스크

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SearchAroundLocation.generated.h"

class UPathFollowingComponent;
struct FPathFollowingResult;

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

		// 진행 중인 이동 상태를 멤버에 두므로 AI마다 인스턴스를 따로 쓴다.
		bCreateNodeInstance = true;
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

	virtual EBTNodeResult::Type AbortTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;

private:
	void OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void OnWaitFinished();
	void UnbindMoveFinished();

	/** 이동 완료 후 탐색하며 머무는 시간 */
	static constexpr float PostMoveWaitSeconds = 2.f;

	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	TWeakObjectPtr<UPathFollowingComponent> CachedPathFollowing;
	FAIRequestID MoveRequestId;
	FDelegateHandle MoveFinishedHandle;
	FTimerHandle WaitTimerHandle;
};
