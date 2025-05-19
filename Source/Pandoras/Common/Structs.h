// Common/Structs.h
// 구조체 모음
#pragma once

#include "CoreMinimal.h"
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