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