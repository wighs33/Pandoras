// ��ġ �ֺ� Ž�� �½�ũ

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

		// �� Task�� �񵿱Ⱑ �ƴϹǷ� �ٷ� ����
		bNotifyTick = false;
	}

	/** �����忡 ����� ���� Ű */
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector SearchLocationKey;

	/** �ݰ� */
	float Radius = 1000.f;

	/** AI�� �������� �󸶳� ������ ���� �������� �������� */
	float AcceptanceRadius = 5.f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	) override;
};
