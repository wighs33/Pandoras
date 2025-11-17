// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_LockOff.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "Interface/CharacterInterface.h"

void UGA_LockOff::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData
)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // --- 캐릭터의 유효성 체크-----------------------------------------------
    ACharacter* Character = GetCharacter();
    if (IsValid(Character))
    {
        // --- 이동 방향으로 캐릭터 회전을 자동으로 맞추기--------------------------
        Character->GetCharacterMovement()->bOrientRotationToMovement = true;

        // --- 이동 모드 원상복구----------------------------------------------
        ICharacterInterface::Execute_ResetMovementMode(Character);

        // --- 락 타겟 제거-----------------------------------------------
        ICharacterInterface::Execute_ClearLockTarget(Character);

        // --- 오너에 락온 GE 해제 (락온 상태 태그)------------------------------------------------
        FGameplayTagContainer WithGrantedTags;
        WithGrantedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.State.Locked")));
        this->BP_RemoveGameplayEffectFromOwnerWithGrantedTags(WithGrantedTags, -1);

        // --- 종료 ---------------------------------
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEnd*/true, /*bWasCancelled*/false);
    }
}