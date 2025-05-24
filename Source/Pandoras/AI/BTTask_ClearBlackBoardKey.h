// ������ Ű ���� �½�ũ

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTree.h" 
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BTTask_ClearBlackBoardKey.generated.h"

UCLASS()
class PANDORAS_API UBTTask_ClearBlackBoardKey : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ClearBlackBoardKey() { NodeName = TEXT("Clear"); }

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override
	{
		if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent())
		{
			// ������ Ű�� �� �����
			BB->ClearValue(Key.SelectedKeyName);
			return EBTNodeResult::Succeeded;
		}
		return EBTNodeResult::Failed;
	}

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector Key;
};
