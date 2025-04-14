// Common/Enums.h

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECharacterFocusPoint : uint8
{
    Main UMETA(DisplayName = "Main"),
    Weapon UMETA(DisplayName = "Weapon"),
    Head UMETA(DisplayName = "Head"),
    Center UMETA(DisplayName = "Center"),
    Feet UMETA(DisplayName = "Feet"),
};