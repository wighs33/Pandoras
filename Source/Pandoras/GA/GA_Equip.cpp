// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/GA_Equip.h"
#include "Interface/ItemWielderInterface.h"
#include "Interface/CharacterInterface.h"
#include "Interface/CharacterGameAbilityInterface.h"
#include "Item/ItemBase.h"

void UGA_Equip::ActivateAbility(
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

	// --- 아이템 생성 후 장착 (SpawnActorFromClass -> ItemWielderInterface.EquipItem) ---
	if (ItemClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = Avatar;
		SpawnParams.Instigator = Cast<APawn>(Avatar);

		AItemBase* NewItem = GetWorld()->SpawnActor<AItemBase>(ItemClass, FTransform::Identity, SpawnParams);
		if (NewItem && Avatar->GetClass()->ImplementsInterface(UItemWielderInterface::StaticClass()))
		{
			IItemWielderInterface::Execute_EquipItem(Avatar, NewItem);
		}
	}

	// --- 오너에게 GE 적용 (self: GA_Pandoras::ApplyGameplayEffects) ---
	// 서버에서만 호출
	if (ActorInfo && ActorInfo->IsNetAuthority())
	{
		this->ApplyGameplayEffects();
	}

	// --- 시청각 이펙트 재생 (MakeGameplayCueParameters + ExecuteGameplayCue_Replicated) ---
	bool bIsPlayer = false;
	if (Avatar->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		bIsPlayer = ICharacterInterface::Execute_IsPlayer(Avatar);
	}

	if (bIsPlayer)
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = Avatar->GetActorLocation();

		if (Avatar->GetClass()->ImplementsInterface(UCharacterGameAbilityInterface::StaticClass()))
		{
			ICharacterGameAbilityInterface::Execute_ExecuteGameplayCue_Replicated(
				Avatar,              // self(Target)
				Avatar,              // TargetActor
				GameplayCueTag,
				CueParams);
		}
	}

	// --- 주어진 어빌리티들 부여 (self: GA_Pandoras::GrantAbilities) ---
	if (ActorInfo && ActorInfo->IsNetAuthority())
	{
		this->GrantAbilities();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, /*bReplicateEnd*/true, /*bWasCancelled*/false);
}
