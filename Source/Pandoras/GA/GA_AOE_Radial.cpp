// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_AOE_Radial.h"

#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameplayTagContainer.h"

#include "Item/ItemBase.h"
#include "SpecialAttacks/AreaOfEffect_RadialImpact.h"
#include "Interface/ItemWielderInterface.h"
#include "Interface/CharacterGameAbilityInterface.h"

void UGA_AOE_Radial::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// --- 대검이나 검을 장착 중인 지 체크 -------------------------------------------
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	FGameplayTagContainer Allowed;
	Allowed.AddTag(FGameplayTag::RequestGameplayTag(FName("Item.Weapon.Sword")));
	Allowed.AddTag(FGameplayTag::RequestGameplayTag(FName("Item.Weapon.GreatSword")));

	if (!ASC->HasAnyMatchingGameplayTags(Allowed)) return;

	// --- 검을 땅으로 내리치는 애니메이션 ---------------------------------
	UAbilityTask_PlayMontageAndWait* MontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			AOEMontage
		);

	MontageTask->OnCompleted.AddDynamic(this, &UGA_AOE_Radial::OnMontageFinished);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_AOE_Radial::OnMontageFinished);
	MontageTask->ReadyForActivation();

	// --- 이펙트시작 노티파이 대기 ------------------------------------------
	UAbilityTask_WaitGameplayEvent* EventTask =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			FGameplayTag::RequestGameplayTag(FName("Notifier.LaunchEffect"))
		);

	EventTask->EventReceived.AddDynamic(this, &UGA_AOE_Radial::OnLaunchEffectEventReceived);
	EventTask->ReadyForActivation();

	// --- 오너의 검 가져와서 검 차징 이펙트 재생 ----------------------------------
	AActor* Avatar = GetAvatarActorFromActorInfo();
	AActor* Weapon = IItemWielderInterface::Execute_GetWeapon(Avatar);

	ICharacterGameAbilityInterface::Execute_ExecuteGameplayCue_Replicated(
		Avatar,
		Weapon,
		FGameplayTag::RequestGameplayTag(FName("GameplayCue.AirCharge")),
		FGameplayCueParameters()
	);
}

void UGA_AOE_Radial::OnMontageFinished()
{
	// --- 종료 -----------------
	K2_EndAbility();
}

void UGA_AOE_Radial::OnLaunchEffectEventReceived(FGameplayEventData /*Payload*/)
{
	// --- 범위 공격 액터 스폰 -------------------------------------
	AActor* Avatar = GetAvatarActorFromActorInfo();

	const FVector SpawnLoc = Avatar->GetActorLocation() + FVector(0.f, 0.f, -80.f);
	const FTransform SpawnTM(FRotator::ZeroRotator, SpawnLoc, FVector(1.f, 1.f, 1.f));

	AAreaOfEffect_RadialImpact* Impact = nullptr;
	if (AOEImpactClass)
	{
		Impact = GetWorld()->SpawnActor<AAreaOfEffect_RadialImpact>(AOEImpactClass, SpawnTM);
		if (Impact)
		{
			// --- 범위 공격 실행 -----------------------------
			Impact->DeployAttack(Avatar);
		}
	}

	// --- 범위공격 시청각 이펙트 재생 -------------------------------------
	FGameplayCueParameters Params;
	Params.Location = SpawnLoc;

	ICharacterGameAbilityInterface::Execute_ExecuteGameplayCue_Replicated(
		Avatar,
		Avatar,
		FGameplayTag::RequestGameplayTag(FName("GameplayCue.AOE.Radial")),
		Params
	);
}