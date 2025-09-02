// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialAttacks/AreaOfEffect_RadilalImpact.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AAreaOfEffect_RadilalImpact::AAreaOfEffect_RadilalImpact()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Ring"));
	AOE_FX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_AOE"));
	MagicExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("MagicExplosion_Cue"));

	RootComponent = DefaultSceneRoot;
	Ring->SetupAttachment(DefaultSceneRoot);
	AOE_FX->SetupAttachment(DefaultSceneRoot);
	MagicExplosionSound->SetupAttachment(DefaultSceneRoot);
}

void AAreaOfEffect_RadilalImpact::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 트리거 바인딩
	Ring->OnComponentBeginOverlap.AddDynamic(this, &AAreaOfEffect_RadilalImpact::OnRingBeginOverlap);
}
