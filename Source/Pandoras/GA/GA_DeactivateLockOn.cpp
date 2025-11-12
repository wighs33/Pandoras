// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_DeactivateLockOn.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

void UGA_DeactivateLockOn::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
    UAbilitySystemComponent* ASC =
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

    // --- 락오프 어빌리티 실행 -------------------------------------------------------
    FGameplayTagContainer LockOffTags;
    LockOffTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.LockOff")));
    if (ASC)
    {
        ASC->TryActivateAbilitiesByTag(LockOffTags);
    }

    // --- 오너에 락온 GE 제거 -----------------------------------------------------------
    FGameplayTagContainer GrantedTags;
    GrantedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Locked")));
    BP_RemoveGameplayEffectFromOwnerWithGrantedTags(GrantedTags);

    // --- 종료 -------------------------------------------------------------------
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEnd*/true, /*bWasCancelled*/false);
}