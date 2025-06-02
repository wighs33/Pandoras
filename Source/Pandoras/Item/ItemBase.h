// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ItemInterface.h"
#include "Common/Enums.h"
#include "ItemBase.generated.h"

class USkeletalMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class PANDORAS_API AItemBase : public AActor, public IItemInterface
{
	GENERATED_BODY()

public:
	AItemBase();

// 컴포넌트
protected:
	// 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// 아이템 스켈레탈 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SK_Item;

// 변수
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayTagContainer Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	EItem ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> GE_Damage_Charged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> GE_Damage_lite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> GE_Stun;
};
