#include "GA/GA_TakeDown_Sword.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "InputAction.h"
#include "GameFramework/HUD.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTagContainer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetMathLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "Item/ItemBase.h"

#include "Interface/CharacterInterface.h"
#include "Interface/PlayerControllerInterface.h"
#include "Interface/HudInterface.h"
#include "Interface/ItemWielderInterface.h"
#include "Interface/ItemInterface.h"


void UGA_TakeDown_Sword::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (TriggerEventData) RunFromEvent(TriggerEventData);  // 이벤트 활성화 경로
    else                  RunNormal();                      // 일반 활성화 경로
}

bool UGA_TakeDown_Sword::CheckVictim() const
{
    if (!Victim) return false;

    // --- 상대가 사망했는 지 체크 ---------------------------------------------------
    UAbilitySystemComponent* VictimASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Victim);
    bool bVictimHasDead = false;
    if (VictimASC)
    {
        FGameplayTagContainer DeadTagContainer;
        DeadTagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Dead")));
        bVictimHasDead = VictimASC->HasAnyMatchingGameplayTags(DeadTagContainer);
    }

    if (bVictimHasDead) return false;

    // --- 오너와 상대와의 거리가 400 아래인지 체크 -------------------------------------
    const AActor* Avatar = GetAvatarActorFromActorInfo();
    const float Dist = Victim->GetDistanceTo(const_cast<AActor*>(Avatar));
    const bool bNearEnough = (Dist < 400.0f);

    return bNearEnough;
}

void UGA_TakeDown_Sword::ShowTip()
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!Avatar) return;

    // --- 오너가 플레이어인 지 체크 ---------------------------------------------------
    bool bIsPlayer = false;
    bIsPlayer = ICharacterInterface::Execute_IsPlayer(Avatar);

    if (!bIsPlayer) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(Avatar, 0);
    if (!PC) return;

    // --- 피니시 어택에 매핑된 키 얻기 ----------------------------------------------------
    FString KeyName;
    KeyName = IPlayerControllerInterface::Execute_GetKeyName(PC, FinishAttackAction);

    // --- HUD의 팁 내용 설정 -------------------------------------------------------------
    const FString TipText = FString::Printf(TEXT("Stun Attack (<Input>%s</>)"), *KeyName);

    if (AHUD* HUD = PC->GetHUD())
    {
        IHudInterface::Execute_ShowTip(HUD, TipText, 2.0f);
    }
}

void UGA_TakeDown_Sword::RunNormal()
{

    // --- 유효성 체크, 검 장착 체크 ---------------------------
    if (!CheckVictim() || !EquipWeapon())
    {
        // --- 상대 저장 후 종료 ------------------------
        Victim = nullptr;
        K2_EndAbility();
        return;
    }

    // --- 공격시드 초기화 -------------------------------------
    if (AttackSeed == 0) AttackSeed = FMath::RandRange(0, 10);

    // --- 오너는 피니셔 공격 애니메이션 재생 (TODO : 적절한 피니셔 공격 애니메이션 필요) ---------
    const FPairedMontages& Pair = FinishAttackMontages[0];

    if (Pair.Attacker)
    {
        UAbilityTask_PlayMontageAndWait* Task =
            UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
                this,
                /*TaskInstanceName*/ NAME_None,
                /*MontageToPlay*/    Pair.Attacker,
                /*Rate*/             1.f,
                /*StartSection*/     NAME_None,
                /*bStopWhenAbilityEnds*/ false
            );

        if (Task)
        {
            Task->OnInterrupted.AddDynamic(this, &UGA_TakeDown_Sword::OnMontageFinish);
            Task->ReadyForActivation();
        }
    }

    // --- 대상은 피니셔 공격 피격 애니메이션 재생 ------------------------------------
    if (Victim && Pair.Victim)
    {
        ICharacterInterface::Execute_PlayMontageReplicated(
            Victim,
            /*AnimMontage*/    Pair.Victim,
            /*InPlayRate*/     1.f,
            /*StartSectionName*/ NAME_None
        );
    }
    AActor* someactor;
    Victim = someactor;

    ++AttackSeed;

    // --- 오너가 상대를 향해 돌아봄 -----------------------------------------------
    AActor* Avatar = GetAvatarActorFromActorInfo();
    const FVector AvatarLocation = Avatar->GetActorLocation();
    const FVector VictimLocation = Victim->GetActorLocation();
    const FRotator LookAtRotation =
        UKismetMathLibrary::FindLookAtRotation(AvatarLocation, VictimLocation);
    const FRotator NewRotation(0.f, LookAtRotation.Yaw, 0.f);
    Avatar->SetActorRotation(NewRotation);

    // --- 칼 꽂는 시점 (노티파이에서 전송) -----------------------------
    UAbilityTask_WaitGameplayEvent* StabInTask =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
            this,
            FGameplayTag::RequestGameplayTag(FName("Notifier.Attack.StabIn"))
        );

    StabInTask->EventReceived.AddDynamic(
        this,
        &UGA_TakeDown_Sword::ShowBlood
    );

    StabInTask->ReadyForActivation();

    // --- 칼 빼는 시점 (노티파이에서 전송) -----------------------------
    UAbilityTask_WaitGameplayEvent* StabOutTask =
        UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
            this,
            FGameplayTag::RequestGameplayTag(FName("Notifier.Attack.StabIOut"))
        );

    StabOutTask->EventReceived.AddDynamic(
        this,
        &UGA_TakeDown_Sword::Die
    );

    StabOutTask->ReadyForActivation();
}

void UGA_TakeDown_Sword::ShowBlood(FGameplayEventData Payload)
{
    // --- 오너의 무기에 출혈 이펙트 재생 -------------------------------------------
    AActor* Avatar = GetAvatarActorFromActorInfo();
    UObject* WeaponObject = IItemWielderInterface::Execute_GetWeapon(Avatar);
    IItemInterface::Execute_TriggerBlood(WeaponObject);
}

void UGA_TakeDown_Sword::Die(FGameplayEventData Payload)
{
    ShowBlood(Payload);

    FTimerHandle Handle;

    GetWorld()->GetTimerManager().SetTimer(
        Handle,
        FTimerDelegate::CreateLambda([this]()
            {
                // --- 타이밍 맞춰 대상에게 사망 어빌리티 이벤트 전송 ---------------------
                FGameplayTagContainer DeathTags;
                DeathTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.Death")));

                ActivateAbilityOnServer(
                    /*TargetActor*/ Victim,
                    /*Tags*/       DeathTags
                );

                // --- 상대에게 부여된 무기 GE 제거 (Item.Weapon) --------------------------
                UAbilitySystemComponent* VictimASC =
                    UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Victim);

                if (VictimASC)
                {
                    FGameplayTagContainer WeaponTags;
                    WeaponTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Item.Weapon")));

                    VictimASC->RemoveActiveEffectsWithGrantedTags(WeaponTags);
                }

                // --- 오너에게 경험치 GE 적용 ----------------------------
                UAbilitySystemComponent* AvatarASC = GetAbilitySystemComponentFromActorInfo();
                if (AvatarASC && GE_MinorExperienceClass)
                {
                    ApplyGameplayEffectToOwnerInCode(GE_MinorExperienceClass);
                }

                // --- 어빌리티 종료 -------------------------------------
                K2_EndAbility();
            }),
        /*Delay*/ 0.3f,
        /*bLoop*/ false
    );
}

void UGA_TakeDown_Sword::RunFromEvent(const FGameplayEventData* EventData)
{
    // --- 피해상대 저장 --------------------------------------------------------
    Victim = EventData ? const_cast<AActor*>(EventData->Instigator.Get()) : nullptr;

    // --- 유효성 체크(스태미나, 대상 상태 등) ---------------------------------------
    if (!CheckVictim())
    {
        K2_EndAbility();
        return;
    }

    // --- 피니시 어택 가능하다는 팁UI 표시
    ShowTip();

    // --- 종료 ----------
    K2_EndAbility();
}

void UGA_TakeDown_Sword::OnMontageFinish()
{
    K2_EndAbility();
}


bool UGA_TakeDown_Sword::EquipWeapon()
{
    // --- 오너가 검 장착 중인지 체크 -------------------------------------------
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC) return false;
    const bool bHasWeapon = ASC->HasMatchingGameplayTag(WeaponTag);

    if (!bHasWeapon)
    {
        // --- 오너는 장착 실행 ----------------------------------------------------
        {
            FGameplayTagContainer EquipTags;
            EquipTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Event.Equip")));
            ASC->TryActivateAbilitiesByTag(EquipTags, /*bAllowRemoteActivation=*/true);
        }

        FTimerHandle Handle;
        GetWorld()->GetTimerManager().SetTimer(
            Handle,
            [&]()
            {
                return ASC->HasMatchingGameplayTag(WeaponTag);
            },
            0.4f,
            /*bLoop=*/false
        );
    }

    return true;
}