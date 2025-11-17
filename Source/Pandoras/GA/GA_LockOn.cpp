// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_LockOn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemGlobals.h"

#include "Interface/CharacterInterface.h"
#include "Common/Enums.h"

void UGA_LockOn::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	OnDelayCompleted();
}

void UGA_LockOn::ScheduleDelay()
{
	// --- 0.7초 마다 반복 실행 --------------------------------------------------
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnDelayCompleted");
	LatentInfo.UUID = __LINE__;

	UKismetSystemLibrary::Delay(this, 0.7f, LatentInfo);
}

void UGA_LockOn::OnDelayCompleted()
{
	Step_LockOnTick();
	ScheduleDelay();
}

void UGA_LockOn::Step_LockOnTick()
{
	// --- 가까운 적 유효성 체크, 유효하면 저장 ---------------------------
	AActor* OutClosest = nullptr;
	bool bLeftOrRight = false;

	GetClosestEnemy(OutClosest, bLeftOrRight);

	if (IsValid(OutClosest))
	{
		AActor* ClosestEnemy = OutClosest;

		// --- 캐릭터 유효성 체크 ----------------------------------------
		ACharacter* Character = GetCharacter();
		if (IsValid(Character))
		{
			// --- 가까운 적을 락 타겟으로 설정 ----------------------------------------
			ICharacterInterface::Execute_SetLockTarget(Character, ClosestEnemy);

			// --- 이동 방향으로 캐릭터 회전을 자동으로 맞추지 않기 ----------------------------
			if (UCharacterMovementComponent* Move = Character->GetCharacterMovement())
			{
				Move->bOrientRotationToMovement = false;
			}

			// --- 현재 이동 모드가 Crouch를 제외한 모드에서는 Walk로 변경하여 설정 ----------------------
			const ECustomMovementMode Mode = ICharacterInterface::Execute_GetMovementMode(Character);

			ECustomMovementMode NewMode = ECustomMovementMode::Walk;
			if (Mode == ECustomMovementMode::Crouch) NewMode = ECustomMovementMode::Crouch;

			ICharacterInterface::Execute_SetMovementMode(Character, NewMode);

			// --- 오너에 락온 GE 적용 (락온 태그) ------------------------
			BP_ApplyGameplayEffectToOwner(LockOnEffectClass, /*Level*/1, /*Stacks*/1);
		}
	}
	else
	{
		// --- 락 오프 실행 ----------------------------------------------
		ACharacter* Character = GetCharacter();
		if (IsValid(Character))
		{
			if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character))
			{
				FGameplayTagContainer Container;
				Container.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.LockOff")));

				ASC->TryActivateAbilitiesByTag(Container, /*bAllowRemoteActivation*/ true);
			}
		}
	}
}