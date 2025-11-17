#include "GA/GA_Attack_Sword.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "AbilitySystemGlobals.h"

#include "Interface/CharacterInterface.h"
#include "Interface/ItemWielderInterface.h"

void UGA_Attack_Sword::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar || !GetWorld())
	{
		// 안전하게 종료
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEnd*/true, /*bWasCancelled*/true);
		return;
	}

	// 애니메이션 중단 플래그 OFF
	Interrupted = false;

	// 오너가 가까운 적을 향해 대시
	AdjustPosition();

	// 플레이어인지 체크---------------------------------------------------------------------------
	bool bIsPlayer = false;
	if (Avatar->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		bIsPlayer = ICharacterInterface::Execute_IsPlayer(Avatar);
	}
	
	// 공격 신호 표시------------------------------------------------------------------------------
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (!bIsPlayer)
	{
		const FGameplayTag Charge =
			FGameplayTag::RequestGameplayTag(TEXT("Character.State.ChargeAttacking"));
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(Charge);

		bool bIsNonBlockable = false;
		if (ASC)
		{
			bIsNonBlockable = ASC->HasAnyMatchingGameplayTags(TagContainer);
		}

		if (Avatar->GetClass()->ImplementsInterface(UItemWielderInterface::StaticClass()))
		{
			IItemWielderInterface::Execute_NotifyAttack(Avatar, bIsNonBlockable);
		}

		if (UAbilityTask_WaitDelay* Task = UAbilityTask_WaitDelay::WaitDelay(this, 0.3f))
		{
			Task->OnFinish.AddDynamic(this, &UGA_Attack_Sword::OnDelayFinished_GetAttackState);
			Task->ReadyForActivation();
		}
	}
	else
	{
		OnDelayFinished_GetAttackState();
	}

	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEnd*/true, /*bWasCancelled*/false);
}

void UGA_Attack_Sword::OnDelayFinished_GetAttackState()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!SCREEN_WARN(Avatar)) return;

	// 공격 중 검 위치 판정 (공격 애니메이션에서 노티파이로 left/right 설정)---------------------------------------
	EAttackState AttackState{};
	if (Avatar->GetClass()->ImplementsInterface(UItemWielderInterface::StaticClass()))
	{
		AttackState = IItemWielderInterface::Execute_GetAttackState(Avatar);
	}

	TObjectPtr<UAnimMontage> FromAttackState = nullptr;
	if (AttackState == static_cast<EAttackState>(0) /*Right*/)
	{
		if (LeftAttack.Num() > 0)
		{
			const int32 Idx = FMath::RandRange(0, LeftAttack.Num() - 1);
			FromAttackState = LeftAttack[Idx];
		}
	}
	else /*Left*/
	{
		FromAttackState = RightAttack;
	}

	// 가까운 적 위치 판정-----------------------------------------------------------------------------------
	bool bLeftOrRight;
	GetClosestEnemy(ClosestEnemy, bLeftOrRight);

	TObjectPtr<UAnimMontage> MontageToPlay = bLeftOrRight ? FromAttackState : RightAttack;
	if (!MontageToPlay)
	{
		return; // 재생할 몽타주가 없으면 종료
	}

	// 판정에 따라 다른 공격 애니메이션 재생------------------------------------------------------------------
	UAbilityTask_PlayMontageAndWait* PMW =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			/*OwningAbility*/ this,
			/*TaskInstanceName*/ NAME_None,
			/*Montage*/ MontageToPlay,
			/*Rate*/ 1.f,
			/*StartSection*/ NAME_None,
			/*bStopWhenAbilityEnds*/ false,
			/*AnimRootMotionTranslationScale*/ 1.f,
			/*StartTimeSeconds*/ 0.f,
			/*bAllowInterruptAfterBlendOut*/ false);

	if (!SCREEN_WARN(PMW)) return;

	PMW->OnCompleted.AddDynamic(this, &UGA_Attack_Sword::OnPMW_Completed);
	PMW->OnInterrupted.AddDynamic(this, &UGA_Attack_Sword::OnPMW_Interrupted);
	PMW->OnCancelled.AddDynamic(this, &UGA_Attack_Sword::OnPMW_Cancelled);

	PMW->ReadyForActivation();

	// 오너에 공격 중 GE 적용 (공격 중 태그)--------------------------------------------------------------------
	if (AttackEffectClass)
	{
		BP_ApplyGameplayEffectToOwner(AttackEffectClass, /*Level*/1.f, /*Stacks*/1);
	}

	// 오너에 블로킹 GE 적용해제--------------------------------------------------------------------------------
	FGameplayTagContainer WithGrantedTags;
	WithGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Blocking")));

	const int32 StacksToRemove = -1; // -1 = 전부 제거
	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(WithGrantedTags, StacksToRemove);

	// 이벤트를 받았는 지 체크 (노티파이에서 전송)--------------------------------------------------------------
	const FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(TEXT("Notifier.AttackLanded"));

	UAbilityTask_WaitGameplayEvent* WaitTask =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			EventTag,
			/*OptionalExternalTarget=*/nullptr,
			/*OnlyTriggerOnce=*/false,
			/*OnlyMatchExact=*/true);

	if (WaitTask)
	{
		WaitTask->EventReceived.AddDynamic(this, &UGA_Attack_Sword::OnAttackLanded);
		WaitTask->ReadyForActivation();
	}
}

void UGA_Attack_Sword::OnPMW_Completed()
{
	K2_EndAbility();
}

void UGA_Attack_Sword::OnPMW_Interrupted()
{
	Interrupted = true;

	// 공격GE 적용 해제 후 종료--------------------------------------------------------------------
	FGameplayTagContainer WithGrantedTags;
	WithGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Attacking")));

	const int32 StacksToRemove = -1; // -1 = 전부 제거
	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(WithGrantedTags, StacksToRemove);

	K2_EndAbility();
}

void UGA_Attack_Sword::OnPMW_Cancelled()
{
	// 공격GE 적용 해제 후 종료---------------------------------------------------------------------
	FGameplayTagContainer WithGrantedTags;
	WithGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Attacking")));

	const int32 StacksToRemove = -1; // -1 = 전부 제거
	BP_RemoveGameplayEffectFromOwnerWithGrantedTags(WithGrantedTags, StacksToRemove);

	K2_EndAbility();
}

void UGA_Attack_Sword::OnAttackLanded(const FGameplayEventData /*Payload*/)
{
	// 차징 공격 플래그 저장--------------------------------------------------------------------------------
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayTagContainer ChargeContainer;
	ChargeContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.ChargeAttacking")));

	IsChargeAttack = (ASC && ASC->HasAnyMatchingGameplayTags(ChargeContainer));

	// 애니메이션 중단 여부 체크 후 카메라 쉐이크--------------------------------------------------------------
	if (!Interrupted)
	{
		const FGameplayTag CueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Attack"));
		// 게임플레이 큐에서 카메라 쉐이크 적용됨
		K2_ExecuteGameplayCue(CueTag, FGameplayEffectContextHandle());
		// 오너가 가까운 적을 향해 돌아봄
		AdjustRotation();
	}

	// 차징 공격 여부에 따라 다른 시간에 종료------------------------------------------------------------------
	const float DelaySeconds = IsChargeAttack ? 0.4f : 0.0f;

	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, DelaySeconds);
	if (DelayTask)
	{
		DelayTask->OnFinish.AddDynamic(this, &UGA_Attack_Sword::OnDelayFinished);
		DelayTask->ReadyForActivation();
	}
}

void UGA_Attack_Sword::OnDelayFinished()
{
	K2_EndAbility();
}