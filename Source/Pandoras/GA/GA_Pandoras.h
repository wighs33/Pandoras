// 루트 어빌리티

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Pandoras.generated.h"


UCLASS()
class PANDORAS_API UGA_Pandoras : public UGameplayAbility
{
	GENERATED_BODY()

// RPC
protected:
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void ActivateAbilityOnServer(AActor* TargetActor, FGameplayTagContainer Tags);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_ActivateAbilityOnServer(AActor* TargetActor, FGameplayTagContainer Tags);

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "C++")
	void GetClosestEnemy(AActor*& ClosestEnemy, bool& bLeftOrRight);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "C++", meta = (ReturnDisplayName = "Character"))
	ACharacter* GetCharacter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void GrantAbilities();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void ApplyGameplayEffects();
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 FactionId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayEffect>> AppliedGameplayEffects;
};
