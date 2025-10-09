// 검으로 공격하는 로직 어빌리티

#pragma once

#include "Pandoras.h"
#include "GA/GA_Pandoras.h"
#include "GA_Attack_Sword.generated.h"

UCLASS()
class PANDORAS_API UGA_Attack_Sword : public UGA_Pandoras
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void AdjustPosition();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void AdjustRotation();

private:
    UFUNCTION() void OnDelayFinished_GetAttackState();
    UFUNCTION() void OnPMW_Completed();
    UFUNCTION() void OnPMW_Interrupted();
    UFUNCTION() void OnPMW_Cancelled();
    UFUNCTION() void OnAttackLanded(const FGameplayEventData Payload);
    UFUNCTION() void OnDelayFinished();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TObjectPtr<UAnimMontage>> LeftAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> RightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	AActor* ClosestEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool Interrupted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool IsChargeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float DashRange;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> AttackEffectClass;
};
