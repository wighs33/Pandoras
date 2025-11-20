#include "GA/GA_Unequip.h"
#include "AbilitySystemComponent.h"

#include "Interface/ItemWielderInterface.h"
#include "Interface/CharacterGameAbilityInterface.h"

void UGA_Unequip::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // --- 오너는 아이템 장착 해제 --------------------------------------
    AActor* AvatarActor = GetAvatarActorFromActorInfo();
    if (!AvatarActor) return;

    IItemWielderInterface::Execute_UnequipItem(AvatarActor);

    // --- 락온 비활성화 어빌리티 실행 ------------------------------------------------
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC) return;

    FGameplayTagContainer NoLockTag;
    NoLockTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.DeactivateLockOn")));
    ASC->TryActivateAbilitiesByTag(NoLockTag, /*bAllowRemoteActivation*/true);

    // --- 무기 태그 제거 -------------------------------------------------
    ASC->RemoveActiveEffectsWithGrantedTags(WeaponTags);

    // --- 시청각 이펙트 재생 ----------------------------------------
    FGameplayCueParameters CueParams;
    CueParams.Location = AvatarActor->GetActorLocation();
    ICharacterGameAbilityInterface::Execute_ExecuteGameplayCue_Replicated(
        AvatarActor,          // self
        AvatarActor,          // TargetActor
        GameplayCueTag,       // GamplayCueTag
        CueParams             // Parameters
    );

    // --- 종료 -----------------------------------------------------------------------------------------------------
    EndAbility(Handle, ActorInfo, ActivationInfo, /*bWasCancelled=*/false, /*bReplicateEnd=*/false);
}