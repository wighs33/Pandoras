// Common/Enums.h
// 열거형 모음

#pragma once

#include "CoreMinimal.h"

// 캐릭터 전신 중 포커싱 부분
UENUM(BlueprintType)
enum class ECharacterFocusPoint : uint8
{
    Main UMETA(DisplayName = "Main"),
    Weapon UMETA(DisplayName = "Weapon"),
    Head UMETA(DisplayName = "Head"),
    Center UMETA(DisplayName = "Center"),
    Feet UMETA(DisplayName = "Feet"),
};

// 아이템
UENUM(BlueprintType)
enum class EItem : uint8
{
    UnArmed UMETA(DisplayName = "UnArmed"),
    Gauntlet UMETA(DisplayName = "Gauntlet"),
    Sword UMETA(DisplayName = "Sword"),
    GreatSword UMETA(DisplayName = "GreatSword"),
    Bow UMETA(DisplayName = "Bow"),
    Helmet UMETA(DisplayName = "Helmet"),
    Armor UMETA(DisplayName = "Armor"),
    Shoes UMETA(DisplayName = "Shoes"),
};

// 공격 중 검 위치
UENUM(BlueprintType)
enum class EAttackState : uint8
{
    Right UMETA(DisplayName = "Right"),
    Left UMETA(DisplayName = "Left")
};

// 이동 모드
UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
    Crouch UMETA(DisplayName = "Crouch"),
    Walk UMETA(DisplayName = "Walk"),
    Run UMETA(DisplayName = "Run"),
    Sprint UMETA(DisplayName = "Sprint"),
};