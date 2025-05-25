// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"

// Sets default values
AItemBase::AItemBase()
{
	SK_Item = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_Item"));
	RootComponent = SK_Item;
}

