// 루트 어빌리티

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Pandoras.generated.h"


UCLASS()
class PANDORAS_API UGA_Pandoras : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "C++")
	void GetClosestEnemy(AActor*& ClosestEnemy, bool& bLeftOrRight);
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;
};
