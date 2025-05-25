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

	// 2) �����忡�� ���� ��������
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(SearchLocationKey.SelectedKeyName);

	// 3) ���� �� ���� ���� ���� ���
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Pawn->GetWorld());
	if (!NavSystem)
	{
		return EBTNodeResult::Failed;
	}
	FNavLocation RandomLoc;
	NavSystem->GetRandomReachablePointInRadius(Origin, Radius, RandomLoc);

	// 4) �̵�
	const EPathFollowingRequestResult::Type MoveResult = AICon->MoveToLocation(RandomLoc.Location, AcceptanceRadius, false);

	// 5) �̵� �Ϸ� ���ε� : �̵� �Ϸ� �� 2�� ��ٸ��� ������ȯ
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

	// 6) InProgress ��ȯ
	return EBTNodeResult::InProgress;
}