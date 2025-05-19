// ��Ʈ �����Ƽ

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
	void ActivateAbilityOnServer(AActor* TargetActor, FGameplayTagContainer Tag);

	// �ӽ�
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_ActivateAbilityOnServer(AActor* TargetActor, FGameplayTagContainer Tag);

	// ���� .h�� ���Ǵ� ������ .cpp���� ����
	bool ActivateAbilityOnServer_Validate(AActor* TargetActor, FGameplayTagContainer Tag);
	void ActivateAbilityOnServer_Implementation(AActor* TargetActor, FGameplayTagContainer Tag);

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "C++")
	void GetClosestEnemy(AActor*& ClosestEnemy, bool& bLeftOrRight);

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "C++", meta = (ReturnDisplayName = "Character"))
	ACharacter* GetCharacter();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void GrantAbilities();
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 FactionId;
};
