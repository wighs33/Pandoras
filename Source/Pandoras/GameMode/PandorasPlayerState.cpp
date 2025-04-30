// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PandorasPlayerState.h"
#include "Item/ItemBase.h"
#include "Net/UnrealNetwork.h"

TArray<TSubclassOf<AItemBase>> APandorasPlayerState::GetItemClasses()
{
    return ItemClasses;
}

void APandorasPlayerState::AddItemClass(TSubclassOf<AItemBase> item)
{
    ItemClasses.AddUnique(item);
    //__debugbreak();
}

void APandorasPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // ItemClasses 변수를 복제 목록에 추가
    DOREPLIFETIME(APandorasPlayerState, ItemClasses);
}
