// AI���� ��Ʈ�ѷ�

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
	// ������
	APandorasAIController();

protected:
	// Ÿ�� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "ClosestActor"))
	AActor* GetClosestDetectedActor();

	// ������ ������ �����ֱ�
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "ClosestActor"))
	void ToggleLastSnapshot(AActor* TargetActor, bool ShowOrHide);

	// ������ �ִ� npc�鿡�� ���
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++", meta = (ReturnDisplayName = "ClosestActor"))
	void AlertNearbyNPCs(AActor* DetectedEnemy);

// ������Ʈ
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> Hearing;

// ����
protected:
	// ���� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> CurrentTarget;

	// ������ ��� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<AActor*> DetectedTargets;

	// ���������� �� ��ġ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<ALastSeen> LastSeenSnapshot;
};
