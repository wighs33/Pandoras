// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SearchAroundLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "TimerManager.h"

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
	if (!BB)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = BB->GetValueAsVector(SearchLocationKey.SelectedKeyName);

	// 3) 범위 내 랜덤 도달 지점 계산
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Pawn->GetWorld());
	if (!NavSystem)
	{
		return EBTNodeResult::Failed;
	}
	FNavLocation RandomLoc;
	if (!NavSystem->GetRandomReachablePointInRadius(Origin, Radius, RandomLoc))
	{
		return EBTNodeResult::Failed;
	}

	// 4) 이동
	const EPathFollowingRequestResult::Type MoveResult = AICon->MoveToLocation(RandomLoc.Location, AcceptanceRadius, false);
	if (MoveResult == EPathFollowingRequestResult::Failed)
	{
		return EBTNodeResult::Failed;
	}
	if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return EBTNodeResult::Succeeded;
	}

	UPathFollowingComponent* PathFollowing = AICon->GetPathFollowingComponent();
	if (!PathFollowing)
	{
		return EBTNodeResult::Failed;
	}

	// 5) 이동 완료 바인딩 : 이동 완료 후 2초 기다리고 성공반환
	CachedOwnerComp = &OwnerComp;
	CachedPathFollowing = PathFollowing;
	MoveRequestId = AICon->GetCurrentMoveRequestID();
	MoveFinishedHandle = PathFollowing->OnRequestFinished.AddUObject(this, &UBTTask_SearchAroundLocation::OnMoveFinished);

	// 6) InProgress 반환
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_SearchAroundLocation::AbortTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory
)
{
	const bool bWasMoving = MoveFinishedHandle.IsValid();
	UnbindMoveFinished();

	UPathFollowingComponent* PathFollowing = CachedPathFollowing.Get();
	if (bWasMoving && PathFollowing && PathFollowing->GetStatus() != EPathFollowingStatus::Idle)
	{
		PathFollowing->AbortMove(*this, FPathFollowingResultFlags::OwnerFinished, MoveRequestId);
	}

	if (UWorld* World = OwnerComp.GetWorld())
	{
		World->GetTimerManager().ClearTimer(WaitTimerHandle);
	}

	return EBTNodeResult::Aborted;
}

void UBTTask_SearchAroundLocation::OnMoveFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!RequestID.IsEquivalent(MoveRequestId))
	{
		return;
	}

	UnbindMoveFinished();

	UBehaviorTreeComponent* OwnerComp = CachedOwnerComp.Get();
	if (!OwnerComp)
	{
		return;
	}

	if (!Result.IsSuccess())
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
		return;
	}

	OwnerComp->GetWorld()->GetTimerManager().SetTimer(
		WaitTimerHandle,
		this,
		&UBTTask_SearchAroundLocation::OnWaitFinished,
		PostMoveWaitSeconds,
		false
	);
}

void UBTTask_SearchAroundLocation::OnWaitFinished()
{
	if (UBehaviorTreeComponent* OwnerComp = CachedOwnerComp.Get())
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_SearchAroundLocation::UnbindMoveFinished()
{
	if (UPathFollowingComponent* PathFollowing = CachedPathFollowing.Get())
	{
		PathFollowing->OnRequestFinished.Remove(MoveFinishedHandle);
	}

	MoveFinishedHandle.Reset();
}