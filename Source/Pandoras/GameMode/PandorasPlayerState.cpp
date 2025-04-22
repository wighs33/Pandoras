// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PandorasPlayerState.h"
#include "Item/ItemBase.h"

TArray<TSubclassOf<AItemBase>> APandorasPlayerState::GetItemClasses()
{
    return ItemClasses;
}

void APandorasPlayerState::AddItemClass(TSubclassOf<AItemBase> item)
{
    ItemClasses.AddUnique(item);
}
