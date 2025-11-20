#include "GA/GA_ParryReact.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "TimerManager.h"

#include "Interface/CharacterGameAbilityInterface.h"

void UGA_ParryReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // --- 휘청거리는 애니메이션 재생 ----------------------------------------
    if (ParryReactMontage)
    {
        UAbilityTask_PlayMontageAndWait* Task =
            UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
                this,
                NAME_None,
                ParryReactMontage
            );

        if (Task)
        {
            Task->ReadyForActivation();
        }
    }

    // --- 패링 성공 이펙트 재생 -----------------------------------------------
    AActor* AvatarActor = GetAvatarActorFromActorInfo();
    FGameplayCueParameters CueParams;
    if (AvatarActor)
    {
        CueParams.Location = AvatarActor->GetActorLocation();
    }

    const FGameplayTag ParryCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Parry"));

    if (AvatarActor)
    {
        ICharacterGameAbilityInterface::Execute_ExecuteGameplayCue_Replicated(
            AvatarActor,
            AvatarActor,
            ParryCueTag,
            CueParams
        );
    }

    // --- 강한 스턴 GE (스턴 태그) ------------------------------------------------
    ApplyGameplayEffectToOwnerInCode(GE_StunAttack);

    // --- 스턴 대기 후 종료 ----------------------------------------------------
    if (UWorld* World = GetWorld())
    {
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(
            TimerHandle,
            [this]()
            {
                K2_EndAbility();
            },
            3.0f,
            false
        );
    }
}