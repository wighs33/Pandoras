// 락오프 태스크

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "AIController.h"

#include "Interface/CharacterInterface.h"

#include "BTTask_ClearLockTarget.generated.h"

UCLASS()
class PANDORAS_API UBTTask_ClearLockTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ClearLockTarget() { NodeName = TEXT("ClearLockTarget"); }

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override
	{
        // 1) AIController와 Pawn 가져오기
		APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
		if (!Pawn)
		{
			return EBTNodeResult::Failed;
		}

        // 2) Pawn이 LockOffEnemy 인터페이스를 구현했으면 호출
        if (Pawn->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
        {
            // LockOffEnemy 이벤트 호출
            ICharacterInterface::Execute_ClearLockTarget(Pawn);
        }

        // 3) 항상 성공으로 마무리
        return EBTNodeResult::Succeeded;
	}
};
