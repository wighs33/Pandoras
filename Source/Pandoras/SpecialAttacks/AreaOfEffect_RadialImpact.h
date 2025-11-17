// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "AreaOfEffect_RadialImpact.generated.h"

class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class PANDORAS_API AAreaOfEffect_RadialImpact : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAreaOfEffect_RadialImpact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void DeployAttack(AActor* InAttacker);
protected:
	// 컴포넌트 생성 직후 호출
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRingBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEnlargeTimelineUpdate(float Size);

    UFUNCTION()
    void HandleEnlargeTimelineFinished();


	bool CheckFaction(UObject* InAttacker, UObject* InVictim) const;


// 컴포넌트
protected:
	// 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// 아이템 스켈레탈 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UStaticMeshComponent> Ring;

	// 범위 공격 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UNiagaraComponent> AOE_FX;

	// 사운드 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAudioComponent> MagicExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> Attacker;

	UPROPERTY()
	FTimeline EnlargeTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<class UCurveFloat> EnlargeCurve;
};
