// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LastSeen.h"

// Sets default values
ALastSeen::ALastSeen()
{
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SK_Snapshot = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Snapshot"));

    RootComponent = DefaultSceneRoot;
    SK_Snapshot->SetupAttachment(DefaultSceneRoot);
}

