// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SearchAroundLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

EBTNodeResult::Type UBTTask_SearchAroundLocation::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory
)
{
	// 1) AIController & Pawn
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon)
		return EBTNodeResult::Failed;

	APawn* Pawn = AICon->GetPawn();
	if (!Pawn)
		return EBTNodeResult::Failed;

	// 2) 블랙보드에서 원점 가져오기
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(SearchLocationKey.SelectedKeyName);

	// 3) 범위 내 랜덤 도달 지점 계산
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Pawn->GetWorld());
	if (!NavSystem)
	{
		return EBTNodeResult::Failed;
	}
	FNavLocation RandomLoc;
	NavSystem->GetRandomReachablePointInRadius(Origin, Radius, RandomLoc);

	// 4) 이동
	const EPathFollowingRequestResult::Type MoveResult = AICon->MoveToLocation(RandomLoc.Location, AcceptanceRadius, false);

	// 5) 이동 완료 바인딩 : 이동 완료 후 2초 기다리고 성공반환
	FDelegateHandle Handle = AICon->GetPathFollowingComponent()->OnRequestFinished.AddLambda(
		[&](FAIRequestID RequestID, const FPathFollowingResult& Result)
		{
			AICon->GetPathFollowingComponent()->OnRequestFinished.Remove(Handle);

			FTimerHandle DelayHandle;
			OwnerComp.GetWorld()->GetTimerManager().SetTimer(
				DelayHandle,
				[&]()
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				},
				2.0f,
				false
			);
		}
	);

	// 6) InProgress 반환
	return EBTNodeResult::InProgress;
}