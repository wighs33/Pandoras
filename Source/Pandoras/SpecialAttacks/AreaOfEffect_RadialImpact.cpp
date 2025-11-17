// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialAttacks/AreaOfEffect_RadialImpact.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

#include "Interface/CharacterGameAbilityInterface.h"
#include "Interface/FactionsInterface.h"

AAreaOfEffect_RadialImpact::AAreaOfEffect_RadialImpact()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Ring"));
	AOE_FX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NS_AOE"));
	MagicExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("MagicExplosion_Cue"));

	RootComponent = DefaultSceneRoot;
	Ring->SetupAttachment(DefaultSceneRoot);
	AOE_FX->SetupAttachment(DefaultSceneRoot);
	MagicExplosionSound->SetupAttachment(DefaultSceneRoot);
}

void AAreaOfEffect_RadialImpact::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 트리거 바인딩
	Ring->OnComponentBeginOverlap.AddDynamic(this, &AAreaOfEffect_RadialImpact::OnRingBeginOverlap);

	if (EnlargeCurve)
	{
		FOnTimelineFloat UpdateFunc;
		UpdateFunc.BindUFunction(this, FName("HandleEnlargeTimelineUpdate"));

		FOnTimelineEvent FinishedFunc;
		FinishedFunc.BindUFunction(this, FName("HandleEnlargeTimelineFinished"));

		EnlargeTimeline.AddInterpFloat(EnlargeCurve, UpdateFunc, FName("Size"));
		EnlargeTimeline.SetTimelineFinishedFunc(FinishedFunc);
		EnlargeTimeline.SetLooping(false);
	}
}

void AAreaOfEffect_RadialImpact::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	EnlargeTimeline.TickTimeline(DeltaSeconds);
}

void AAreaOfEffect_RadialImpact::OnRingBeginOverlap_Implementation(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// --- 오버랩된 액터가 공격자와 다른 지 체크 -------------------------------
	if (!Attacker) return;
	if (!OtherActor) return;
	if (OtherActor == Attacker) return;

	// --- 적 아군 구분 ---------------------------------------------------
	if (!CheckFaction(Attacker, OtherActor)) return;

	// --- 피격자의 피격 어빌리티로 공격자를 이벤트로 보내기 ----------------------------
	const FGameplayTag ChargedTag = FGameplayTag::RequestGameplayTag(FName("Character.Event.HitReact.Charged"));

	FGameplayEventData Payload;
	Payload.Instigator = this;

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerCharacter) return;

	ICharacterGameAbilityInterface::Execute_SendGameplayEvent_Replicated(
		PlayerCharacter,
		OtherActor,
		ChargedTag,
		Payload
	);
}

bool AAreaOfEffect_RadialImpact::CheckFaction(UObject* InAttacker, UObject* InVictim) const
{
	if (!InAttacker || !InVictim) return false;

	const int32 VictimFaction = IFactionsInterface::Execute_GetFactionId(InVictim);
	const int32 AttackerFaction = IFactionsInterface::Execute_GetFactionId(InAttacker);

	const bool bIsEnemy = AttackerFaction != VictimFaction;
	return bIsEnemy;
}

void AAreaOfEffect_RadialImpact::DeployAttack_Implementation(AActor* InAttacker)
{
	// --- 공격자 저장 ---------------------
	Attacker = InAttacker;

	// --- 일정시간동안 수치 변화 갱신 -------------------------
	EnlargeTimeline.PlayFromStart();
}

void AAreaOfEffect_RadialImpact::HandleEnlargeTimelineUpdate(float Size)
{
	if (!Ring) return;

	// --- 사이즈 설정 --------------------------------------
	const float ScaleXY = Size * 3.f;
	Ring->SetRelativeScale3D(FVector(ScaleXY, ScaleXY, 1.f));
}

void AAreaOfEffect_RadialImpact::HandleEnlargeTimelineFinished()
{
	// --- 충돌설정 없애고 파괴 ------------------------------------
	if (Ring)
	{
		Ring->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SetLifeSpan(5.f);
}