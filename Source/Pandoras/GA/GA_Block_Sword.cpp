// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Block_Sword.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitCancel.h"
#include "Animation/AnimMontage.h"
#include "GameplayEffect.h"
#include "TimerManager.h"

#include "Item/ItemBase.h"

#include "Interface/ItemWielderInterface.h"
#include "Interface/ItemInterface.h"

void UGA_Block_Sword::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    // --- 오너의 무기 유효성 체크 -------------------------------------------------
    AActor* Avatar = GetAvatarActorFromActorInfo();
    AActor* Weapon = IItemWielderInterface::Execute_GetWeapon(Avatar);

    if (!IsValid(Weapon))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEnd*/true, /*bWasCancelled*/true);
        return;
    }

    // --- 무기 저장 후 블로킹 박스 활성화 ------------------------------------------------
    Sword = Weapon;
    IItemInterface::Execute_ToggleBlockBox(Sword, /*Enable*/true);

    // --- 블로킹 애니메이션 -----------------------------------------------------
    if (BlockMontage)
    {
        BlockMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this, NAME_None, BlockMontage);

        if (BlockMontageTask)
        {
            BlockMontageTask->ReadyForActivation();
        }
    }

    // --- 오너에 블로킹 GE 적용 ------------------------------------------------
    ApplyGameplayEffectToOwnerInCode(GE_Block);

    // --- 취소 입력 대기 -----------------------------------------------------
    if (auto* WaitCancel = UAbilityTask_WaitCancel::WaitCancel(this))
    {
        WaitCancel->OnCancel.AddDynamic(this, &UGA_Block_Sword::HandleCancel);
        WaitCancel->ReadyForActivation();
    }

    // --- 블로킹 대기 후 다음 진행 -----------------------------------------------
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            BlockTimerHandle,
            this,
            &UGA_Block_Sword::K2_EndAbility,
            5.0f, false);
    }
}

void UGA_Block_Sword::HandleCancel()
{
    // --- 블로킹 애니메이션 마무리 -------------------------------------------------
    const FName EndSection(TEXT("End"));
    if (BlockMontage && BlockMontage->IsValidSectionName(EndSection))
    {
        // 앞선 Task가 살아 있으면 Ability가 끝날 때 같은 몽타주를 멈춰 End 구간이 잘린다.
        if (IsValid(BlockMontageTask))
        {
            BlockMontageTask->EndTask();
        }

        if (auto* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this, NAME_None, BlockMontage, 1.f, EndSection, /*bStopWhenAbilityEnds*/ false))
        {
            Task->ReadyForActivation();
        }
    }

    K2_EndAbility();
}

void UGA_Block_Sword::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility,
    bool bWasCancelled)
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(BlockTimerHandle);
    }

    // --- 오너에 블로킹GE 적용해제 -----------------------------------------------
    RemoveGameplayEffectFromOwnerInCode(TEXT("Character.State.Blocking"));

    // --- 블로킹 박스 비활성화 ------------------------------------------------
    if (IsValid(Sword))
    {
        IItemInterface::Execute_ToggleBlockBox(Sword, /*Enable*/false);
    }

    BlockMontageTask = nullptr;

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
