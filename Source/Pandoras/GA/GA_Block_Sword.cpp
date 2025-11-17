// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Block_Sword.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitCancel.h"
#include "GameplayEffect.h"
#include "TimerManager.h"

#include "Item/ItemBase.h"

// 너의 프로젝트 인터페이스
#include "Interface/ItemWielderInterface.h"
#include "Interface/ItemInterface.h"

void UGA_Block_Sword::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

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
        if (auto* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this, NAME_None, BlockMontage))
        {
            Task->ReadyForActivation();
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
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(
            TimerHandle,
            [this]()
            {
                RemoveBlockEffectAndFinish();
            },
            5.0f, false);
    }
}

void UGA_Block_Sword::HandleCancel()
{
    // --- 블로킹 애니메이션 마무리 -------------------------------------------------
    if (BlockMontage)
    {
        if (auto* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this, NAME_None, BlockMontage, 1.f, FName(TEXT("End"))))
        {
            Task->ReadyForActivation();
        }
    }

    RemoveBlockEffectAndFinish();
}

void UGA_Block_Sword::RemoveBlockEffectAndFinish()
{
    // --- 오너에 블로킹GE 적용해제 -----------------------------------------------
    RemoveGameplayEffectFromOwnerInCode(TEXT("Character.State.Blocking"));

    // --- 블로킹 박스 비활성화 ------------------------------------------------
    if (Sword)
    {
        IItemInterface::Execute_ToggleBlockBox(Sword, /*Enable*/false);
    }

    K2_EndAbility();
}
