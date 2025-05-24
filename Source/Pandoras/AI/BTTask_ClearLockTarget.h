// ������ �½�ũ

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
        // 1) AIController�� Pawn ��������
		APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
		if (!Pawn)
		{
			return EBTNodeResult::Failed;
		}

        // 2) Pawn�� LockOffEnemy �������̽��� ���������� ȣ��
        if (Pawn->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
        {
            // LockOffEnemy �̺�Ʈ ȣ��
            ICharacterInterface::Execute_ClearLockTarget(Pawn);
        }

        // 3) �׻� �������� ������
        return EBTNodeResult::Succeeded;
	}
};
