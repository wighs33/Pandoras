// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AreaOfEffect_RadilalImpact.generated.h"

class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class PANDORAS_API AAreaOfEffect_RadilalImpact : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAreaOfEffect_RadilalImpact();

protected:
	// ������Ʈ ���� ���� ȣ��
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRingBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void DeployAttack(AActor* Attacker);

// ������Ʈ
protected:
	// ��Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// ������ ���̷�Ż �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Ring;

	// ���� ���� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> AOE_FX;

	// ���� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> MagicExplosionSound;

};
