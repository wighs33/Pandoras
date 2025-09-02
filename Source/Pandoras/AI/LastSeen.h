// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LastSeen.generated.h"

UCLASS()
class PANDORAS_API ALastSeen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALastSeen();

protected:
	// ���� ����߸���
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ToggleSnapshot(bool NewShowOrHide);

protected:
	// ��Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// ������ ���̷��� �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SK_Snapshot;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool ShowOrHide;
};
