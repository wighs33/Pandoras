// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Pandoras.h"
#include "Net/UnrealNetwork.h"

bool UGA_Pandoras::ActivateAbilityOnServer_Validate(AActor* TargetActor, FGameplayTagContainer Tag)
{
    return true;
}

void UGA_Pandoras::ActivateAbilityOnServer_Implementation(AActor* TargetActor, FGameplayTagContainer Tag)
{
    BP_ActivateAbilityOnServer(TargetActor, Tag);
}
