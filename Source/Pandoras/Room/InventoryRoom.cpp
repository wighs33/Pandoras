#include "Room/InventoryRoom.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/HUD.h"

// 인터페이스
#include "Interface/HudInterface.h"
#include "Interface/CharacterInterface.h"
#include "Interface/ItemWielderInterface.h"

// GA/ASC
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// 컴포넌트들을 생성하고 계층정립, 세부 설정은 에디터에서 처리
AInventoryRoom::AInventoryRoom()
{
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    MainSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SP_Main"));
    WeaponSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SP_Weapon"));
    HeadSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SP_Head"));
    HandSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SP_Hand"));
    FootSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SP_Foot"));
    DomeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Dome"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    RotateCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RotateCapsule"));
    PlayerPosition = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPosition"));

    RootComponent = DefaultSceneRoot;
    DomeMesh->SetupAttachment(DefaultSceneRoot);
    PlayerPosition->SetupAttachment(DefaultSceneRoot);
    MainSpringArm->SetupAttachment(PlayerPosition);
    WeaponSpringArm->SetupAttachment(PlayerPosition);
    HeadSpringArm->SetupAttachment(PlayerPosition);
    HandSpringArm->SetupAttachment(PlayerPosition);
    FootSpringArm->SetupAttachment(PlayerPosition);
    Camera->SetupAttachment(MainSpringArm);
    RotateCapsule->SetupAttachment(Camera);
}

void AInventoryRoom::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // 캡슐 클릭 시점 바인딩
    RotateCapsule->OnClicked.AddDynamic(this, &AInventoryRoom::OnCapsuleClicked);
    RotateCapsule->OnReleased.AddDynamic(this, &AInventoryRoom::OnCapsuleReleased);
}

// 플레이 시작 시 플레이어 컨트롤러의 입력 활성화
void AInventoryRoom::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        EnableInput(PC);
    }
}

// 인벤토리 입장<->퇴장 래퍼함수
bool AInventoryRoom::TogglePauseMode_Implementation()
{
	ACharacter* PlayerCharacter_local = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!SCREEN_WARN(PlayerCharacter_local)) return false;

	bool bAlive = true;
	if (PlayerCharacter_local->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
	{
		bAlive = ICharacterInterface::Execute_IsAlive(PlayerCharacter_local);
	}
	if (!SCREEN_WARN(bAlive)) return false;

	if (bInPauseMode)
	{
		ExitPauseMode();
		return false;
	}
	else
	{
		EnterPauseMode();
		return true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 입력 액션일 때 호출 함수

// 인벤토리 창 열기 입력
void AInventoryRoom::OnInventoryStarted_Implementation()
{
	if(!TogglePauseMode()) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!SCREEN_WARN(PC)) return;
	if (AHUD* HUD = PC->GetHUD())
	{
		if (HUD->GetClass()->ImplementsInterface(UHudInterface::StaticClass()))
		{
			IHudInterface::Execute_SwitchToInventoryUI(HUD, this);
		}
	}
}

// 스킬트리 창 열기 입력
void AInventoryRoom::OnSkillTreeStarted_Implementation()
{
	if (!TogglePauseMode()) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!SCREEN_WARN(PC)) return;
	if (AHUD* HUD = PC->GetHUD())
	{
		if (HUD->GetClass()->ImplementsInterface(UHudInterface::StaticClass()))
		{
			IHudInterface::Execute_SwitchToSkillMenu(HUD);
		}
	}
}

// 회전 활성화 시 시선(look) 입력
void AInventoryRoom::OnLookTriggered_Implementation(float XValue)
{
	if (!bMeshRotating) return;

	const float DeltaSeconds = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.f;
	const float DeltaYaw = XValue * DeltaSeconds * LookYawSpeed;

	if (!SCREEN_WARN(PlayerCharacter)) return;
	if (PlayerCharacter && PlayerCharacter->GetMesh())
	{
		// 메시 회전 중 플래그 ON일 때 메시를 프레임에 비례하여 회전
		PlayerCharacter->GetMesh()->AddRelativeRotation(FRotator(0.f, DeltaYaw, 0.f), false, nullptr, ETeleportType::None);

		// 타이머 종료 시 메시 회전 리셋
		GetWorldTimerManager().SetTimer(
			MeshResetTimerHandle,
			this, &AInventoryRoom::ResetMeshRotation,
			ResetDelaySeconds,
			/*bLoop=*/false
		);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 인벤토리에 입장
void AInventoryRoom::EnterPauseMode_Implementation()
{
	// 플레이어 캐릭터 저장------------------------------------------------------
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!SCREEN_WARN(PlayerCharacter)) return;

	// 락온 해제 GA 활성화 (AbilitySystem)------------------------------------------------------
	if (DeactivateLockOnAbilityClass && IsValid(PlayerCharacter))
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerCharacter))
		{
			ASC->TryActivateAbilityByClass(DeactivateLockOnAbilityClass, /*bAllowRemoteActivation=*/true);
		}
	}

	// 캐릭터 입력 비활성 & 3D 클릭 이벤트 활성------------------------------------------------------
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->DisableInput(PC);
		}
		PC->bEnableClickEvents = true;
	}

	// 이동 정지 & 루트모션 종료까지 대기------------------------------------------------------
	if (IsValid(PlayerCharacter))
	{
		if (UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			MoveComp->DisableMovement();
		}
	}

	// 움직임 비활성화------------------------------------------------------
	UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement();
	if (!SCREEN_WARN(MoveComp)) return;
	MoveComp->DisableMovement();

	PollRootMotionUntilStopped(); // 이후 단계는 루트모션이 끝나면 계속
}

// 루트모션이 끝날 때까지 기다리기------------------------------------------------------
void AInventoryRoom::PollRootMotionUntilStopped()
{
	// 타이머를 사용해 재귀적으로 반복
	if (PlayerCharacter->HasAnyRootMotion())
	{
		GetWorldTimerManager().SetTimer(
			RootMotionCheckHandle,
			this,
			&AInventoryRoom::PollRootMotionUntilStopped,
			RootMotionCheckInterval,
			false
		);
		return;
	}

	ContinueEnterPause_AfterRootMotion();
}

// 루트모션이 끝난 후 호출
void AInventoryRoom::ContinueEnterPause_AfterRootMotion()
{
	// 인벤토리 모드 On------------------------------------------------------
	bInPauseMode = true;

	// 인게임에서 메시 상대 트랜스폼 저장------------------------------------------------------
	if (!SCREEN_WARN(PlayerCharacter)) return;
	if (USkeletalMeshComponent* Mesh = PlayerCharacter->GetMesh())
	{
		MeshRelativeTransformOrigin = Mesh->GetRelativeTransform();

		// 캐릭터 자체가 아닌 메시만 인벤토리 위치(PlayerPosition)로 이동 (위치+회전)------------
		const FTransform Target = PlayerPosition->GetComponentTransform();
		Mesh->SetWorldLocationAndRotation(
			Target.GetLocation(),
			Target.GetRotation().Rotator(),
			/*bSweep=*/false,
			/*OutHitResult=*/nullptr,
			ETeleportType::None
		);

		// 인벤토리에서의 메시 상대 트랜스폼 저장------------------------------------------------------
		MeshRelativeTransformInventory = Mesh->GetRelativeTransform();
	}

	// 카메라 뷰타겟을 이 액터로 전환------------------------------------------------------
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetViewTargetWithBlend(this, /*BlendTime=*/0.f, EViewTargetBlendFunction::VTBlend_Linear, /*BlendExp=*/0.f, /*bLockOutgoing=*/false);

		// HUD 인터페이스로 UI 입력 활성화------------------------------------------------------
		if (AHUD* HUD = PC->GetHUD())
		{
			if (HUD->GetClass()->ImplementsInterface(UHudInterface::StaticClass()))
			{
				IHudInterface::Execute_ToggleUIInput(HUD, /*bInput=*/true);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 인벤토리에서 퇴장
void AInventoryRoom::ExitPauseMode_Implementation()
{
	// 메시의 회전을 멈추고 캐릭터의 메시를 인게임에서의 위치로 다시 이동----------------------------------------------
	if (!SCREEN_WARN(PlayerCharacter)) return;

	StopMeshRotation();

	if (USkeletalMeshComponent* Mesh = PlayerCharacter->GetMesh())
	{
		const FVector  OriginLoc = MeshRelativeTransformOrigin.GetLocation();
		const FRotator OriginRot = MeshRelativeTransformOrigin.Rotator(); // = GetRotation().Rotator()
		Mesh->SetRelativeLocationAndRotation(
			OriginLoc,
			OriginRot,
			/*bSweep*/ false,
			/*OutHit*/ nullptr,
			/*Teleport*/ ETeleportType::None
		);
	}

	// 캐릭터 입력 다시 활성화하고 캐릭터의 시점으로 다시 전환---------------------------------------------------
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PlayerCharacter->EnableInput(PC);

		PC->SetViewTargetWithBlend(
			PlayerCharacter,
			/*BlendTime*/ 0.f,
			/*BlendFunc*/ EViewTargetBlendFunction::VTBlend_Linear,
			/*BlendExp*/  0.f,
			/*bLockOutgoing*/ false
		);
	}

	bInPauseMode = false;

	// 캐릭터의 움직임을 걷기로 설정--------------------------------------------------------------------------
	if (UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
	}

	// 인게임 HUD로 전환하며 UI입력과 3D클릭 비활성화-----------------------------------------------------------
	if (PC)
	{
		if (AHUD* HudObj = PC->GetHUD())
		{
			if (HudObj->GetClass()->ImplementsInterface(UHudInterface::StaticClass()))
			{
				// 메시지 노드 호출 패턴
				IHudInterface::Execute_SwitchToInGameHud(HudObj);
				IHudInterface::Execute_ToggleUIInput(HudObj, /*bInput*/ false);
			}
		}

		PC->bEnableClickEvents = false;
	}

	// 무기 가지고 있으면 락온 실행-----------------------------------------------------------------------------
	if (PlayerCharacter->GetClass()->ImplementsInterface(UItemWielderInterface::StaticClass()))
	{
		AItemBase* WeaponObj = IItemWielderInterface::Execute_GetWeapon(PlayerCharacter);
		if (WeaponObj)
		{
			if (UAbilitySystemComponent* ASC =
				UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerCharacter))
			{
				const FGameplayTag LockOnTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Event.LockOn"));
				FGameplayTagContainer TagCon;
				TagCon.AddTag(LockOnTag);

				ASC->TryActivateAbilitiesByTag(TagCon, /*bAllowRemoteActivation*/ true);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 카메라 포커싱 변경
void AInventoryRoom::ChangeFocusPoint_Implementation(ECharacterFocusPoint FocusPoint)
{
	// 메시 회전 리셋-----------------------------------------------------------------------------
	ResetMeshRotation();

	// [포커싱 시점 키워드]에 따라 다른 스프링암에 카메라 부착---------------------------------------
	USceneComponent* Parent = nullptr;
	switch (FocusPoint)
	{
	case ECharacterFocusPoint::Main:   Parent = MainSpringArm;   break;
	case ECharacterFocusPoint::Weapon: Parent = WeaponSpringArm; break;
	case ECharacterFocusPoint::Head:   Parent = HeadSpringArm;   break;
	case ECharacterFocusPoint::Hands:  Parent = HandSpringArm;   break;
	case ECharacterFocusPoint::Feet:   Parent = FootSpringArm;   break;
	default:                           Parent = MainSpringArm;   break;
	}

	if (!Camera || !Parent)
	{
		SCREEN_WARN(false);
	}

	FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, /*bWeldSimulatedBodies=*/true);
	Camera->AttachToComponent(Parent, AttachRules);

	// 카메라를 새 부모의 원점으로 서서히 이동---------------------------------------------------
	FLatentActionInfo Latent;
	Latent.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(
		/*Component*/                    Camera,
		/*TargetRelativeLocation*/       FVector::ZeroVector,
		/*TargetRelativeRotation*/       FRotator::ZeroRotator,
		/*bEaseOut*/                     true,
		/*bEaseIn*/                      false,
		/*OverTime*/                     FocusMoveTime,
		/*bForceShortestRotationPath*/   false,
		/*MoveAction*/                   EMoveComponentAction::Move,
		/*LatentInfo*/                   Latent
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 인벤토리 내 회전 관련
// 
// 캡슐을 마우스로 누르고 있을 때
void AInventoryRoom::OnCapsuleClicked_Implementation(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
    bMeshRotating = true;
}

// 캡슐을 눌렀던 걸 뗄 때
void AInventoryRoom::OnCapsuleReleased_Implementation(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
    bMeshRotating = false;
}

// 회전 리셋
void AInventoryRoom::ResetMeshRotation_Implementation()
{
	// if (bInPauseMode) then MoveComponentTo(...)
	if (!bInPauseMode) return;

	SetMeshRotation(EMoveComponentAction::Move);
}

// 회전 중지
void AInventoryRoom::StopMeshRotation_Implementation()
{
	SetMeshRotation(EMoveComponentAction::Stop);
}

// 회전 로직
void AInventoryRoom::SetMeshRotation(EMoveComponentAction::Type Action)
{
	// Stop exec 핀에 해당: MoveComponentTo(..., MoveAction=Stop)
	ACharacter* PC = PlayerCharacter;
	if (!PC) return;

	USceneComponent* Comp = PC->GetMesh();
	if (!Comp) return;

	const FVector  TargetLoc = MeshRelativeTransformInventory.GetLocation();
	const FRotator TargetRot = MeshRelativeTransformInventory.GetRotation().Rotator();

	UKismetSystemLibrary::MoveComponentTo(
		Comp,
		TargetLoc,
		TargetRot,
		/*bEaseOut*/true,
		/*bEaseIn*/true,
		/*OverTime*/0.8f,
		/*bForceShortestRotationPath*/true,
		Action,
		FLatentActionInfo()
	);
}