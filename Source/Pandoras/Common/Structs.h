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

    // 아이템명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    FString Name = TEXT("ItemName");

    // 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Description = TEXT("ItemDescription");

    // 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TSoftObjectPtr<UTexture2D> Icon;

    // 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	EItem ItemType = EItem::UnArmed;
};

class UGameplayEffect;

USTRUCT(BlueprintType)
struct PANDORAS_API FWeaponProperties
{
    GENERATED_BODY()

    // 데미지 GE
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    TSoftClassPtr<UGameplayEffect> GEDamage;

    // 스턴 GE
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    TSoftClassPtr<UGameplayEffect> GEStun;

    // 무기 스탯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    TMap<FString, int> WeaponStats;
};

class UGameplayAbility;

USTRUCT(BlueprintType)
struct PANDORAS_API FEquipmentProperties
{
    GENERATED_BODY()
    
    // GC 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FGameplayTag GameplayCueTag;

    // 장착 GE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TSoftClassPtr<UGameplayEffect> GameplayEffect;

    // 장착 GA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<TSoftClassPtr<UGameplayAbility>> GrantedAbilities;
};

USTRUCT(BlueprintType)
struct PANDORAS_API FItemData : public FTableRowBase
{
    GENERATED_BODY()

    // 아이템 공통 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
    FItemCommonProperty ItemCommonProperty;

    // 무기 속성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FWeaponProperties WeaponProperties;

    // 장비 속성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FEquipmentProperties EquipmentProperties;
};

USTRUCT(BlueprintType)
struct PANDORAS_API FDialogue : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString TalkContent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<UTexture2D> CharacterImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<USoundBase> Voice;
};