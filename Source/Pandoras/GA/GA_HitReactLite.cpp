#include "GA_HitReactLite.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"

#include "Interface/ItemWielderInterface.h"
#include "Interface/ItemInterface.h"
#include "Interface/CharacterGameAbilityInterface.h"
#include "Interface/CharacterInterface.h"
#include "Item/ItemBase.h"

void UGA_HitReactLite::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 공격자 저장 --------------------------------------------------------------------------------------------------------
	if (TriggerEventData)
	{
		Attacker = const_cast<AActor*>(TriggerEventData->Instigator.Get());
	}

	// 오너가 회피 중인 지 체크------------------------------------------------------------------------------------------------
	// 오너(Avatar) ASC 가져오기
	AActor* Avatar = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Avatar);
	if (!Avatar || !GetWorld())
	{
		// 안전하게 종료
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEnd*/true, /*bWasCancelled*/true);
		return;
	}

	FGameplayTagContainer Evading;
	Evading.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Evading")));

	if (ASC->HasAnyMatchingGameplayTags(Evading))
	{
		// 그래프: K2_EndAbility
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEndAbility=*/true, /*bWasCancelled=*/false);
		return;
	}

	// 오너가 블로킹 중인지 체크 -----------------------------------------------------------------------------------------------------
	FGameplayTagContainer Blocking;
	Blocking.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Blocking")));

	if (ASC->HasAnyMatchingGameplayTags(Blocking))
	{
		// 카메라 쉐이크용 GC 실행--------------------------------------------------------------------------------------------------
		{
			const FGameplayTag CueTag = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.ReactHitBlocked"));
			FGameplayEffectContextHandle Ctx = MakeEffectContext(CurrentSpecHandle, CurrentActorInfo);
			K2_ExecuteGameplayCue(CueTag, Ctx);
		}

		// 블로킹 리액트 몽타주 재생------------------------------------------------------------------------------------------------
		if (Reaction_Blocking)
		{
			UAbilityTask_PlayMontageAndWait* Task =
				UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
					this,
					/*TaskInstanceName*/ NAME_None,
					/*Montage*/        Reaction_Blocking,
					/*Rate*/           1.f,
					/*StartSection*/   NAME_None,
					/*bStopWhenEnd*/   false,
					/*RootMotionScale*/1.f,
					/*StartTimeSec*/   0.f,
					/*bAllowIntAfterBlendOut*/ false
				);
			if (Task)
			{
				Task->ReadyForActivation();
			}
		}

		// 패링 가능 여부 체크----------------------------------------------------------------------------------------------------
		if (ASC)
		{
			FGameplayTagContainer ParryEnabled;
			ParryEnabled.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.ParryEnabled")));

			const bool bParryEnabled = ASC->HasAnyMatchingGameplayTags(ParryEnabled);
			if (bParryEnabled)
			{
				// 공격자는 패링반응 어빌리티 실행 -----------------------------------------------------------------------------------
				if (IsValid(Attacker))
				{
					if (UAbilitySystemComponent* AttackerASC =
						UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Attacker))
					{
						FGameplayTagContainer ParryReactTags;
						ParryReactTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.ParryReact")));
						AttackerASC->TryActivateAbilitiesByTag(ParryReactTags, /*bAllowRemoteActivation*/ true);
					}
				}

				// 오너는 피니셔 공격 실행하면서 공격자 정보 전송 -------------------------------------------------------------------------
				if (IsValid(Avatar) &&
					Avatar->GetClass()->ImplementsInterface(UCharacterGameAbilityInterface::StaticClass()))
				{
					const FGameplayTag FinishAttackTag =
						FGameplayTag::RequestGameplayTag(TEXT("Character.Event.FinishAttack"));

					FGameplayEventData Payload;
					Payload.EventTag = FinishAttackTag;
					Payload.Instigator = Attacker;

					ICharacterGameAbilityInterface::Execute_SendGameplayEvent_Replicated(
						Avatar,            // self (대상 오브젝트)
						Avatar,            // Actor 파라미터 (그래프에서 같은 아바타 연결)
						FinishAttackTag,
						Payload
					);
				}
			}
			else
			{
				// 공격자는 블로킹반응 어빌리티 실행 -----------------------------------------------------------------------------------
				if (IsValid(Attacker))
				{
					if (UAbilitySystemComponent* AttackerASC =
						UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Attacker))
					{
						FGameplayTagContainer BlockedReactTags;
						BlockedReactTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.AttackBlockedReact")));
						AttackerASC->TryActivateAbilitiesByTag(BlockedReactTags, /*bAllowRemoteActivation*/ true);
					}
				}
			}
		}
	}
	else
	{
		// 카메라 쉐이크용 GC 실행--------------------------------------------------------------------------------------------------
		{
			const FGameplayTag ReactHitCue = FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.ReactHit"));
			K2_ExecuteGameplayCue(ReactHitCue, FGameplayEffectContextHandle());
		}

		// 공격자는 스태미나 증가 GE 적용-----------------------------------------------------------------------------------------------
		if (IsValid(Attacker))
		{
			if (UAbilitySystemComponent* AttackerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Attacker))
			{
				if (AddStaminaEffectClass)
				{
					FGameplayEffectContextHandle Ctx = AttackerASC->MakeEffectContext();
					AttackerASC->BP_ApplyGameplayEffectToSelf(AddStaminaEffectClass, /*Level*/ 0.f, Ctx);
				}
			}

			// 공격자의 무기를 받아와서 출혈 이펙트 트리거------------------------------------------------------------------------------
			if (Attacker->GetClass()->ImplementsInterface(UItemWielderInterface::StaticClass()))
			{
				if (AItemBase* Weapon = IItemWielderInterface::Execute_GetWeapon(Attacker))
				{
					if (Weapon->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
					{
						IItemInterface::Execute_TriggerBlood(Weapon);
					}
				}
			}

			// Non블로킹 리액션 몽타주 재생 --------------------------------------------------------------------------------------------
			if (Reaction_NonBlocking)
			{
				UAbilityTask_PlayMontageAndWait* Task =
					UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
						this,
						/*TaskInstanceName*/ NAME_None,
						/*Montage*/ Reaction_NonBlocking,
						/*Rate*/ 1.f,
						/*StartSection*/ NAME_None,
						/*bStopWhenAbilityEnds*/ false,
						/*AnimRootMotionTranslationScale*/ 1.f,
						/*StartTimeSeconds*/ 0.f,
						/*bAllowInterruptAfterBlendOut*/ false
					);
				if (Task)
				{
					Task->ReadyForActivation();
				}
			}

			// 지정된 GE 적용--------------------------------------------------------------------------------------------
			ApplyGameplayEffects();
		}

	}

	// 캐릭터 캡슐의 정면이 공격자를 향하도록 부드럽게 회전 (Ease In/Out : 시작과 끝을 느리게 처리)--------------------------------------------
	USceneComponent* Root = Avatar->GetRootComponent();
	if (!Root) { K2_EndAbility(); return; }

	AActor* Target = Attacker;
	if (!Target) { K2_EndAbility(); return; }

	const FVector StartLoc = Avatar->GetActorLocation();
	const FVector TargetLoc = Target->GetActorLocation();
	const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(StartLoc, TargetLoc);
	const FRotator TargetRelRot(0.f, LookAt.Yaw, 0.f); // Roll/Pitch 0

	FLatentActionInfo Latent;
	Latent.CallbackTarget = this;
	Latent.ExecutionFunction = FName(TEXT("OnRotateToAttackerCompleted"));
	Latent.Linkage = 0;
	Latent.UUID = __LINE__; // 임의의 고유 값

	UKismetSystemLibrary::MoveComponentTo(
		Root,
		/*TargetRelativeLocation=*/ Root->GetRelativeLocation(), // 위치는 고정
		/*TargetRelativeRotation=*/ TargetRelRot,
		/*bEaseOut=*/ true,
		/*bEaseIn=*/  true,
		/*OverTime=*/ 0.3f,
		/*bForceShortestRotationPath=*/ false,
		/*MoveAction=*/ EMoveComponentAction::Move,
		/*LatentInfo=*/ Latent
	);
}

void UGA_HitReactLite::OnRotateToAttackerCompleted()
{
	// 딜레이 후 종료-----------------------------------------------------------------------------------------------------------------
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName(TEXT("K2_EndAbility"));
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__; // 임의의 식별자

	UKismetSystemLibrary::Delay(this, /*Duration*/ 0.5f, LatentInfo);

	// 사망 이벤트 대기------------------------------------------------------------------------------------------------------------------
	static const FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("Character.State.Dead"));

	if (UAbilityTask_WaitGameplayTagAdded* WaitDead =
		UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, DeadTag, /*ExternalTarget=*/nullptr, /*OnlyTriggerOnce=*/false))
	{
		WaitDead->Added.AddDynamic(this, &UGA_HitReactLite::OnDeath);
		WaitDead->ReadyForActivation();
	}
}

void UGA_HitReactLite::OnDeath()
{
	const FGameplayEventData& Data = CurrentEventData;
	AActor* InstigatorActor = const_cast<AActor*>(Data.Instigator.Get());

	// 공격자가 플레이어인지 체크---------------------------------------------------------------------------------------------------
	if (InstigatorActor && InstigatorActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		if (ICharacterInterface::Execute_IsPlayer(InstigatorActor))
		{
			K2_EndAbility();
			return;
		}
	}

	// 공격자에 경험치 획득 GE 적용--------------------------------------------------------------------------------------------------------------
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InstigatorActor))
	{
		if (MinorExperienceEffectClass)
		{
			FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
			ASC->BP_ApplyGameplayEffectToSelf(MinorExperienceEffectClass, /*Level*/ 0.f, Ctx);
		}
	}

	// 게임 전체 진행 속도를 느리게 설정-----------------------------------------------------------------------------------------------
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::SetGlobalTimeDilation(World, 0.1f);
	}

	// (공격자 -> 오너의 스켈레탈 메시) 방향으로 힘을 가해 날려 버리기--------------------------------------------------------------------------
	const FGameplayAbilityActorInfo* Info = GetCurrentActorInfo();
	USkeletalMeshComponent* SkelMeshComp =
		(Info && Info->SkeletalMeshComponent.IsValid()) ? Info->SkeletalMeshComponent.Get() : nullptr;

	if (InstigatorActor && SkelMeshComp)
	{
		const FVector From = InstigatorActor->GetActorLocation();
		const FVector To = SkelMeshComp->GetComponentLocation();

		const FVector DirUnit = UKismetMathLibrary::GetDirectionUnitVector(From, To);
		const FVector A(DirUnit.X, DirUnit.Y, 0.2f);
		const FVector Force = A * 500000.0f;

		SkelMeshComp->AddForce(Force, NAME_None, /*bAccelChange=*/true);
	}


	if (UWorld* World = GetWorld())
	{
		FTimerHandle Handle_Reset;
		World->GetTimerManager().SetTimer(
			Handle_Reset,
			FTimerDelegate::CreateUObject(this, &UGA_HitReactLite::ResetTimeAndEnd),
			0.05f, /*bLoop=*/false
		);
	}
	else
	{
		// 월드가 없을 때 즉시 복구 및 종료
		if (UWorld* W = GetWorld())
		{
			UGameplayStatics::SetGlobalTimeDilation(W, 1.0f);
		}
		K2_EndAbility();
	}
}

void UGA_HitReactLite::ResetTimeAndEnd()
{
	// 게임 전체 진행 속도 원상복구----------------------------------------------------
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::SetGlobalTimeDilation(World, 1.0f);
	}

	K2_EndAbility();
}