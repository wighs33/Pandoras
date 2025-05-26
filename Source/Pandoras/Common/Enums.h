// Common/Enums.h
// ������ ����

#pragma once

#include "CoreMinimal.h"

// ĳ���� ���� �� ��Ŀ�� �κ�
UENUM(BlueprintType)
enum class ECharacterFocusPoint : uint8
{
    Main UMETA(DisplayName = "Main"),
    Weapon UMETA(DisplayName = "Weapon"),
    Head UMETA(DisplayName = "Head"),
    Center UMETA(DisplayName = "Center"),
    Feet UMETA(DisplayName = "Feet"),
};

// ������
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

// ���� �� �� ��ġ
UENUM(BlueprintType)
enum class EAttackState : uint8
{
    Right UMETA(DisplayName = "Right"),
    Left UMETA(DisplayName = "Left")
};

// �̵� ���
UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
    Crouch UMETA(DisplayName = "Crouch"),
    Walk UMETA(DisplayName = "Walk"),
    Run UMETA(DisplayName = "Run"),
    Sprint UMETA(DisplayName = "Sprint"),
};