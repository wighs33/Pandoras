// Common/Structs.h
// 구조체 모음
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "GameplayTagContainer.h"
#include "Structs.generated.h"

USTRUCT(BlueprintType)
struct PANDORAS_API FNotification
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
    FString Text;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
    UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct PANDORAS_API FMontage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
    UAnimMontage* AnimMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
    float InPlayRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
    FName StartSectionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
    float TriggerTime;
};

USTRUCT(BlueprintType)
struct PANDORAS_API FPairedMontages
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    TObjectPtr<UAnimMontage> Attacker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    TObjectPtr<UAnimMontage> Victim;
};

class UGA_Pandoras;

USTRUCT(BlueprintType)
struct PANDORAS_API FSkill : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    FString SkillName = TEXT("Skill");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    int32 Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<FName> Dependencies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int32 SkillPointCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TSubclassOf<UGA_Pandoras> SkillGameplayAbility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    FText SkillDescription = FText::FromString(TEXT("Description"));
};

USTRUCT(BlueprintType)
struct PANDORAS_API FItemCommonProperty
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    FString Name = TEXT("ItemName");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Description = TEXT("ItemDescription");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EItem ItemType = EItem::UnArmed;
};

class UGameplayEffect;

USTRUCT(BlueprintType)
struct PANDORAS_API FWeaponProperties
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    float DamageRadius = 80.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TSoftClassPtr<UGameplayEffect> GEDamageCharge;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    TSoftClassPtr<UGameplayEffect> GEDamageLite;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    TSoftClassPtr<UGameplayEffect> GEStun;
};

class UGameplayAbility;

USTRUCT(BlueprintType)
struct PANDORAS_API FEquipmentProperties
{
    GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FGameplayTag GameplayCueTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TSoftClassPtr<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<TSoftClassPtr<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TSoftClassPtr<UGameplayEffect> AppliedgameplayEffects;
};

USTRUCT(BlueprintType)
struct PANDORAS_API FItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    FItemCommonProperty ItemCommonProperty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FWeaponProperties WeaponProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FEquipmentProperties EquipmentProperties;
};