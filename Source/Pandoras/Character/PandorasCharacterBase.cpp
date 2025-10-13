#include "PandorasCharacterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h" 
#include "GameFramework/PlayerState.h"
#include "AIController.h"  
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "CharacterTrajectoryComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/GameStateBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/HUD.h"

#include "Interface/PlayerStateInterface.h"
#include "Interface/CharacterAnimationInterface.h"
#include "Interface/HudInterface.h"
#include "Item/ItemBase.h" 
#include "GA/GA_Equip.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// 생성자
// Skeletal Mesh 및 애니메이션 블루프린트 참조는 블루프린트에서 설정
// 어빌리티 시스템 컴포넌트 생성, 리플리케이션 허용
APandorasCharacterBase::APandorasCharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbiliitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal); 

	CharacterTrajectory = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("CharacterTrajectory"));
}

// 컴포넌트 초기화 직후
// ASC의 어트리뷰트 세트 설정
void APandorasCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(AbilitySystemComponent))
	{
		BaseActorAttributes = AbilitySystemComponent->GetSet<UBaseActorAttributes>();

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseActorAttributes->GetHealthAttribute()).AddUObject(this, &APandorasCharacterBase::HealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseActorAttributes->GetStaminaAttribute()).AddUObject(this, &APandorasCharacterBase::StaminaChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseActorAttributes->GetXPPointsAttribute()).AddUObject(this, &APandorasCharacterBase::XPPointsChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseActorAttributes->GetMaxHealthAttribute()).AddUObject(this, &APandorasCharacterBase::MaxHealthChanged);
	}
}

void APandorasCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	LoadCharacterData();

	GetWorldTimerManager().SetTimer(
		TimerHandle_RefreshUI,
		this,
		&APandorasCharacterBase::RefreshUI,
		/*Delay*/2.f,
		/*bLoop*/false
	);
}

// 이 클래스가 네트워크에 올라가는 시점에 호출
void APandorasCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APandorasCharacterBase, CurrentWeapon);
	DOREPLIFETIME(APandorasCharacterBase, CurrentHelmet);
	DOREPLIFETIME(APandorasCharacterBase, CurrentArmor);
	DOREPLIFETIME(APandorasCharacterBase, CurrentGlove);
	DOREPLIFETIME(APandorasCharacterBase, CurrentShoes);
	DOREPLIFETIME(APandorasCharacterBase, bDead);
	DOREPLIFETIME(APandorasCharacterBase, MontageData);
	DOREPLIFETIME(APandorasCharacterBase, WeaponType);
	DOREPLIFETIME(APandorasCharacterBase, CurrentMovementMode);
}


//////////////////////////////////////////////////////////////////////////
// Input

void APandorasCharacterBase::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APandorasCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APandorasCharacterBase::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APandorasCharacterBase::Look);

		// Custom
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APandorasCharacterBase::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &APandorasCharacterBase::StopAttack);
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &APandorasCharacterBase::LockOn);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &APandorasCharacterBase::Block);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &APandorasCharacterBase::StopBlocking);
		EnhancedInputComponent->BindAction(FinishAttackAction, ETriggerEvent::Started, this, &APandorasCharacterBase::FinishAttack);
		EnhancedInputComponent->BindAction(EvadeAction, ETriggerEvent::Started, this, &APandorasCharacterBase::Evade);
		EnhancedInputComponent->BindAction(ToggleWalkAction, ETriggerEvent::Started, this, &APandorasCharacterBase::ToggleWalk);
		EnhancedInputComponent->BindAction(ToggleCrouchAction, ETriggerEvent::Started, this, &APandorasCharacterBase::ToggleCrouch);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APandorasCharacterBase::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APandorasCharacterBase::Sprint);
		EnhancedInputComponent->BindAction(AreaOfEffectAction, ETriggerEvent::Started, this, &APandorasCharacterBase::AreaOfEffect);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APandorasCharacterBase::Move_Implementation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APandorasCharacterBase::Look_Implementation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

bool APandorasCharacterBase::GiveAndActivateAbility_Server_Validate(TSubclassOf<UGameplayAbility> Ability)
{
	return true;
}

void APandorasCharacterBase::GiveAndActivateAbility_Server_Implementation(TSubclassOf<UGameplayAbility> Ability)
{
	BP_GiveAndActivateAbility_Server(Ability);
}

bool APandorasCharacterBase::OnlyGiveAbility_Server_Validate(TSubclassOf<UGameplayAbility> Ability)
{
	return true;
}

void APandorasCharacterBase::OnlyGiveAbility_Server_Implementation(TSubclassOf<UGameplayAbility> Ability)
{
	BP_OnlyGiveAbility_Server(Ability);
}

void APandorasCharacterBase::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	UpdateHealth(Health);
}

void APandorasCharacterBase::StaminaChanged(const FOnAttributeChangeData& Data)
{
	float Stamina = Data.NewValue;
	UpdateStamina(Stamina);
}

void APandorasCharacterBase::XPPointsChanged(const FOnAttributeChangeData& Data)
{
	float XPPoints = Data.NewValue;
	UpdateXPPoints(XPPoints);
}

void APandorasCharacterBase::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;
	UpdateMaxHealth(MaxHealth);
}

void APandorasCharacterBase::LoadAttributes(TMap<FGameplayAttribute, float> SavedAttributesMap)
{
	for (const TPair<FGameplayAttribute, float>& Pair : SavedAttributesMap)
	{
		FGameplayAttribute Attribute = Pair.Key;
		float Value = Pair.Value;
		AbilitySystemComponent->ApplyModToAttribute(Attribute, EGameplayModOp::Override, Value);
	}
}

void APandorasCharacterBase::EquipItem_Implementation(AItemBase* Item)
{
	if (!Item) return;

	// 아이템 타입에 따라 다른 소켓에 부착----------------------------------------------------------------------------------------
	EItem ItemType = EItem::UnArmed;
	if (Item->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
	{
		ItemType = IItemInterface::Execute_GetItemType(Item);
	}

	if (AActor* AsActor = Cast<AActor>(Item))
	{
		AttachToSocket(AsActor, ItemType);
	}

	// 아이템 별로 저장--------------------------------------------------------------------------------------------------------
	switch (ItemType)
	{
	case EItem::Sword:
		CurrentWeapon = Item;
		// 무기 별 장착 몽타주 재생--------------------------------------------------------------------------------------------
		if (EquipMontage_Sword)
		{
			PlayMontageReplicated(EquipMontage_Sword, /*Rate*/1.f, /*Section*/NAME_None);
		}
		break;
		
	case EItem::GreatSword:
		CurrentWeapon = Item;
		if (EquipMontage_GreatSword)
		{
			PlayMontageReplicated(EquipMontage_GreatSword, /*Rate*/1.f, /*Section*/NAME_None);
		}
		break;

	case EItem::Helmet:
		CurrentHelmet = Item;
		return;

	case EItem::Armor:
		CurrentArmor = Item;
		// 아이템의 뼈를 캐릭터의 뼈에 맞추기 ------------------------------------------------------------------------------
		if (CurrentArmor && CurrentArmor->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
		{
			IItemInterface::Execute_InitLeadPose(CurrentArmor, GetMesh());
		}
		return;

	case EItem::Glove:
		CurrentGlove = Item;
		if (CurrentGlove && CurrentGlove->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
		{
			IItemInterface::Execute_InitLeadPose(CurrentGlove, GetMesh());
		}
		return;

	case EItem::Shoes:
		CurrentShoes = Item;
		if (CurrentShoes && CurrentShoes->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
		{
			IItemInterface::Execute_InitLeadPose(CurrentShoes, GetMesh());
		}
		return;

	case EItem::UnArmed:
	default:
		return;
	}

	GetWorldTimerManager().SetTimer(
		TimerHandle_AttachWeapon,
		this,
		&APandorasCharacterBase::AttachWeaponToRightHand_Deferred,
		/*Delay*/0.5f,
		/*bLoop*/false
	);
}

void APandorasCharacterBase::AttachToSocket_Implementation(AActor* Target, EItem Index)
{
	if (!IsValid(Target) || !IsValid(GetMesh()))
	{
		return;
	}

	FName SocketName = NAME_None;
	switch (Index)
	{
	case EItem::Sword:       SocketName = TEXT("Socket_SwordSheith"); break;
	case EItem::GreatSword:  SocketName = TEXT("Socket_Back");        break;
	case EItem::Helmet:      SocketName = TEXT("Socket_Head");        break;
	default:                 SocketName = NAME_None;                   break;
	}

	const FAttachmentTransformRules Rules(
		EAttachmentRule::SnapToTarget,   // Location
		EAttachmentRule::SnapToTarget,   // Rotation
		EAttachmentRule::KeepRelative,   // Scale
		true
	);

	Target->AttachToComponent(GetMesh(), Rules, SocketName);
}

void APandorasCharacterBase::AttachWeaponToRightHand_Deferred()
{
	// 무기를 손 소켓에 부착 -------------------------------------------------------------------------------
	if (!IsValid(CurrentWeapon) || !IsValid(GetMesh()))
	{
		return;
	}

	const FAttachmentTransformRules Rules(
		EAttachmentRule::SnapToTarget,   // Location
		EAttachmentRule::SnapToTarget,   // Rotation
		EAttachmentRule::KeepRelative,   // Scale
		/*bWeldSimulatedBodies*/ true
	);

	CurrentWeapon->AttachToComponent(GetMesh(), Rules, FName("hand_r"));

	// walk/run을 무기 버전으로 변경 ----------------------------------------------------------------------
	EItem ItemType = EItem::UnArmed;
	if (CurrentWeapon->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
	{
		ItemType = static_cast<EItem>(IItemInterface::Execute_GetItemType(CurrentWeapon));
	}

	UpdateWeapon(ItemType);
}

void APandorasCharacterBase::UpdateWeapon_Implementation(EItem ItemType)
{
	// AnimInstance의 무기 업데이트------------------------------------------------------------------------------
	WeaponType = ItemType;

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
		{
			if (AnimInst->GetClass()->ImplementsInterface(UCharacterAnimationInterface::StaticClass()))
			{
				ICharacterAnimationInterface::Execute_UpdateWeapon(AnimInst, WeaponType);
			}
		}
	}

	// 이동모드 갱신----------------------------------------------------------------------------------------------------
	SetMovementMode(CurrentMovementMode);
}

void APandorasCharacterBase::OnRep_WeaponType_Implementation()
{
	// AnimInstance의 무기 업데이트------------------------------------------------------------------------------
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
		{
			if (AnimInst->GetClass()->ImplementsInterface(UCharacterAnimationInterface::StaticClass()))
			{
				ICharacterAnimationInterface::Execute_UpdateWeapon(AnimInst, WeaponType);
			}
		}
	}
}

void APandorasCharacterBase::UnequipItem_Implementation()
{
	if (!CurrentWeapon)
	{
		return;
	}

	// 무기 타입에 따라 다른 장착 해제 애니메이션 재생------------------------------------------------------------------------------
	EItem ItemType = EItem::UnArmed;
	if (CurrentWeapon->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
	{
		ItemType = IItemInterface::Execute_GetItemType(CurrentWeapon);
	}

	UAnimMontage* Montage = nullptr;
	switch (WeaponType)
	{
	case EItem::Sword:
		Montage = UnequipMontage_Sword;
		break;

	case EItem::GreatSword:
		Montage = UnequipMontage_GreatSword;
		break;

	default:
		Montage = nullptr;
		break;
	}

	PlayMontageReplicated(Montage, /*InPlayRate*/1.f, /*StartSectionName*/NAME_None);

	// walk/run을 무기 해제 버전으로 변경---------------------------------------------------------------------------
	UpdateWeapon(EItem::UnArmed);

	// 아이템 삭제------------------------------------------------------------------------------------------------
	if (CurrentWeapon->GetClass()->ImplementsInterface(UItemWielderInterface::StaticClass()))
	{
		IItemWielderInterface::Execute_DestroyItem(this, ItemType);
	}
}

void APandorasCharacterBase::GiveDefaultAbilities_Implementation()
{
	if (!AbilitySystemComponent) return;

	if (!HasAuthority()) return;

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (!AbilityClass) continue;

		FGameplayAbilitySpec Spec(AbilityClass, /*Level*/0, /*InputID*/-1, /*SourceObj*/this);
		AbilitySystemComponent->GiveAbility(Spec);
	}
}

void APandorasCharacterBase::AddDefaultWeaponAbilities_Implementation()
{
	FTimerDelegate Del = FTimerDelegate::CreateWeakLambda(this, [this]()
				{
					if (!this || !this->AbilitySystemComponent)
					{
						return;
					}

					// 리스트 내 각각의 어빌리티---------------------------------------------------------------------------------------------
					for (const TSubclassOf<UGameplayAbility>& AbilityClass : this->DefaultWeaponAbilities)
					{
						if (!AbilityClass)
						{
							continue;
						}

						// 어빌리티 부여 후 실행------------------------------------------------------------------------------------------
						FGameplayAbilitySpecHandle Handle;
						if (this->HasAuthority())     // 서버에서만 부여
						{
							FGameplayAbilitySpec Spec(AbilityClass, /*Level*/0, /*InputID*/INDEX_NONE);
							Handle = this->AbilitySystemComponent->GiveAbility(Spec);
						}

						if (Handle.IsValid())
						{
							this->AbilitySystemComponent->TryActivateAbility(Handle, /*bAllowRemoteActivation*/true);
						}

						// 기본 아이템을 인벤토리 데이터로 추가-------------------------------------------------------------------------------------------
						if (UClass* Raw = AbilityClass.Get();
							Raw && Raw->IsChildOf(UGA_Equip::StaticClass()))
						{
							TSubclassOf<UGA_Equip> EquipAbilityClass = Raw;
							IItemWielderInterface::Execute_AddItemToPlayerState(this, EquipAbilityClass);
						}
					}
				});
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, Del, 0.2f, /*bLoop*/false);
}

void APandorasCharacterBase::Block_Implementation()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// 블로킹 어빌리티 실행---------------------------------------------------------------------------------------------------------
	FGameplayTagContainer BlockTag;
	BlockTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.Block")));
	AbilitySystemComponent->TryActivateAbilitiesByTag(BlockTag, /*bAllowRemoteActivation*/true);

	// 패링 활성화 어빌리티 실행---------------------------------------------------------------------------------------------------------
	FGameplayTagContainer ParryTag;
	ParryTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Upgrade.Combat.Parry")));
	AbilitySystemComponent->TryActivateAbilitiesByTag(ParryTag, /*bAllowRemoteActivation*/true);
}

void APandorasCharacterBase::StopBlocking_Implementation()
{
	if (!AbilitySystemComponent) return;

	// 입력 취소 이벤트 전송---------------------------------------------------------------------------------------------------------
	AbilitySystemComponent->InputCancel();
}

void APandorasCharacterBase::SetAttackState_Implementation(EAttackState InAttackState)
{
	AttackState = InAttackState;
}

void APandorasCharacterBase::Evade_Implementation()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// 회피 어빌리티 실행---------------------------------------------------------------------------------------------------------
	FGameplayTagContainer EvadeTag;
	EvadeTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.Evade")));
	AbilitySystemComponent->TryActivateAbilitiesByTag(EvadeTag, /*bAllowRemoteActivation*/true);
}

void APandorasCharacterBase::NotifyFootstep_Implementation()
{
	// 빠른 이동모드 일 수록 발소리 더 크게 재생---------------------------------------------------------------------------------
	const float VolumePitch =
		(CurrentMovementMode == ECustomMovementMode::Crouch) ? 0.3f :
		(CurrentMovementMode == ECustomMovementMode::Walk) ? 0.7f :
		(CurrentMovementMode == ECustomMovementMode::Run) ? 1.0f :
		(CurrentMovementMode == ECustomMovementMode::Sprint) ? 1.3f : 1.0f;

	if (FootstepCue)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			FootstepCue,
			GetActorLocation(),
			VolumePitch,   // VolumeMultiplier
			VolumePitch    // PitchMultiplier
		);
	}

	// 쪼그려앉기가 아닌 이동 모드에서는 소음 발생 (소음으로 AI의 감지)-----------------------------------------------------------
	const float Loudness =
		(CurrentMovementMode == ECustomMovementMode::Crouch) ? 0.0f :
		(CurrentMovementMode == ECustomMovementMode::Walk) ? 0.7f :
		(CurrentMovementMode == ECustomMovementMode::Run) ? 1.0f :
		(CurrentMovementMode == ECustomMovementMode::Sprint) ? 1.3f : 0.0f;

	MakeNoise(Loudness, this, GetActorLocation(), 1000.f);
}

void APandorasCharacterBase::AddItemToPlayerState_Implementation(TSubclassOf<UGA_Equip> itemAbilityClass)
{
	if (APlayerState* PS = GetPlayerState())
	{
		// 플레이어 스테이트에 아이템 클래스 저장----------------------------------------------------------------
		if (PS->GetClass()->ImplementsInterface(UPlayerStateInterface::StaticClass()))
		{
			IPlayerStateInterface::Execute_AddItemClass(PS, itemAbilityClass);
		}
	}
}

void APandorasCharacterBase::InitItemLeadPose(AItemBase* Item)
{
	if (!Item) return;

	if (Item->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
	{
		IItemInterface::Execute_InitLeadPose(Item, GetMesh());
	}
}

void APandorasCharacterBase::OnRep_Armor_Implementation()
{
	// 갑옷을 뼈에 맞추기------------------------------------------------------------------------------------------------------
	InitItemLeadPose(CurrentArmor);
}

void APandorasCharacterBase::OnRep_Glove_Implementation()
{
	// 장갑을 뼈에 맞추기------------------------------------------------------------------------------------------------------
	InitItemLeadPose(CurrentGlove);
}

void APandorasCharacterBase::OnRep_Shoes_Implementation()
{
	// 신발을 뼈에 맞추기------------------------------------------------------------------------------------------------------
	InitItemLeadPose(CurrentShoes);
}

void APandorasCharacterBase::SlowDown_Implementation(float Rate, float Duration)
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
	{
		return;
	}

	// 애니메이션 속도 잠시동안 감속 후 원래대로-----------------------------------------------------------------------------
	MeshComp->GlobalAnimRateScale = Rate;

	FTimerDelegate ResetDelegate = FTimerDelegate::CreateWeakLambda(this, [&]()
		{
			MeshComp->GlobalAnimRateScale = 1.0f;
		});

	FTimerHandle Handle;
	const float SafeDuration = FMath::Max(0.f, Duration);
	GetWorldTimerManager().SetTimer(Handle, ResetDelegate, SafeDuration, /*bLoop=*/false);
}

void APandorasCharacterBase::DestroyAIController()
{
	// AI 컨트롤러 파괴 후 소유권 해제----------------------------------------------------------------------------
	if (AAIController* AICon = UAIBlueprintHelperLibrary::GetAIController(this))
	{
		if (IsValid(AICon))
		{
			AICon->SetLifeSpan(0.1f);
			AICon->UnPossess();
		}
	}
}

void APandorasCharacterBase::OnDeath()
{
	// 움직임 비활성화---------------------------------------------------------------------------------------------------
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->DisableMovement();
	}

	// 입력 비활성화---------------------------------------------------------------------------------------------------
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}
	else if (UWorld* World = GetWorld())
	{
		if (APlayerController* FallbackPC = World->GetFirstPlayerController())
		{
			DisableInput(FallbackPC);
		}
	}

	// 하체를 물리 기반으로 움직이기------------------------------------------------------------------------------
	if (USkeletalMeshComponent* Skel = GetMesh())
	{
		Skel->SetAllBodiesBelowSimulatePhysics(TEXT("pelvis"), /*bNewSimulate*/true, /*bIncludeSelf*/true);
	}

	// 캡슐 콜리전 비활성화-----------------------------------------------------------------------------------
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// AI 컨트롤러 파괴---------------------------------------------------------------------------------
	DestroyAIController();

	// 무기 떨어뜨리기------------------------------------------------------------------------------------
	UObject* WeaponObj = nullptr;
	if (GetClass()->ImplementsInterface(UItemWielderInterface::StaticClass()))
	{
		WeaponObj = IItemWielderInterface::Execute_GetWeapon(this);
	}

	if (WeaponObj && WeaponObj->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
	{
		IItemInterface::Execute_Drop(WeaponObj);
	}
}

void APandorasCharacterBase::Die_Implementation()
{
	bDead = true;
	OnDeath();
}

void APandorasCharacterBase::OnRep_Dead_Implementation()
{
	if (bDead) OnDeath();
}

void APandorasCharacterBase::PlayMontageReplicated_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	// 몽타주 데이터 값 변경 (On_Rep 함수 호출 예약)------------------------------------------------------------
	MontageData.AnimMontage = AnimMontage;
	MontageData.InPlayRate = InPlayRate;
	MontageData.StartSectionName = StartSectionName;

	if (AGameStateBase* GS = GetWorld() ? GetWorld()->GetGameState() : nullptr)
	{
		// 서버 시간
		MontageData.TriggerTime = GS->GetServerWorldTimeSeconds();
	}
	ForceNetUpdate(); // 즉시 복제 푸시 유도

	// 몽타주 재생 후 몽타주 값 초기화 (몽타주 끝난 후 On_Rep 함수 호출 예약)-----------------------------------------------
	const float Duration = PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(
		Handle,
		[this]()
		{
			// AnimMontage 만 None으로 바꿔서 On_Rep 재호출 유도
			auto Temp = MontageData;
			Temp.AnimMontage = nullptr;
			MontageData = Temp;
			ForceNetUpdate();
		},
		FMath::Max(Duration, 0.f),
		false
	);
}

void APandorasCharacterBase::OnRep_MontageData_Implementation()
{
	UAnimMontage* MontageToPlay = MontageData.AnimMontage;
	if (!MontageToPlay)
	{
		return;
	}

	// 클라와 서버의 게임 경과 시간 차이. 즉, 네트워크 전송시간을 얻어내어 그 시간만큼 건너띄고 재생하여 싱크맞추기-------------------------------------------
	USkeletalMeshComponent* Skel = GetMesh();
	if (!Skel) return;

	UAnimInstance* AnimInst = Skel->GetAnimInstance();
	if (!AnimInst) return;

	// 서버-클라 시간차만큼 건너뛰기(=네트워크 전송 지연 보정)
	double ServerNow = 0.0;
	if (const AGameStateBase* GS = GetWorld() ? GetWorld()->GetGameState() : nullptr)
	{
		ServerNow = GS->GetServerWorldTimeSeconds();
	}

	const double RawDelta = ServerNow - static_cast<double>(MontageData.TriggerTime);
	const double MaxLen = static_cast<double>(MontageToPlay->GetPlayLength());
	const double StartPos = FMath::Clamp(RawDelta, 0.0, MaxLen);

	// AnimInst->StopAllMontages(0.f);
	AnimInst->Montage_Play(MontageToPlay, MontageData.InPlayRate,
			EMontagePlayReturnType::Duration,
			static_cast<float>(StartPos), /*bStopAllMontages=*/true);

	if (!MontageData.StartSectionName.IsNone())
	{
		AnimInst->Montage_JumpToSection(MontageData.StartSectionName, MontageToPlay);
	}
}

void APandorasCharacterBase::SendGameplayEvent_Replicated_Implementation(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)
{
	// 게임플레이 이벤트로 데이터 전송 동기화
	if (!IsValid(Actor)) return;
	if (HasAuthority())
	{
		SendGameplayEvent_Multicast(Actor, EventTag, Payload);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, Payload);
	}
}

void APandorasCharacterBase::SendGameplayEvent_Multicast_Implementation(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, EventTag, Payload);
}

void APandorasCharacterBase::ExecuteGameplayCue_Replicated_Implementation(AActor* TargetActor, FGameplayTag GamplayCueTag, const FGameplayCueParameters Parameters)
{
	// 게임플레이 큐 동기화
	ExecuteGameplayCue_Server(TargetActor, GamplayCueTag, Parameters);
}

bool APandorasCharacterBase::ExecuteGameplayCue_Server_Validate(AActor* TargetActor, FGameplayTag GamplayCueTag, const FGameplayCueParameters Parameters)
{
	return true;
}

void APandorasCharacterBase::ExecuteGameplayCue_Server_Implementation(AActor* TargetActor, FGameplayTag GamplayCueTag, const FGameplayCueParameters Parameters)
{
	ExecuteGameplayCue_Multicast(TargetActor, GamplayCueTag, Parameters);
}

void APandorasCharacterBase::ExecuteGameplayCue_Multicast_Implementation(AActor* TargetActor, FGameplayTag GamplayCueTag, const FGameplayCueParameters Parameters)
{
	if (!IsValid(TargetActor)) return;
	UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(TargetActor, GamplayCueTag, Parameters);
}

void APandorasCharacterBase::ClearItemAbilities_Implementation(AActor* Item)
{
	if (!AbilitySystemComponent || !Item)
	{
		return;
	}

	// 아이템 태그 얻기-------------------------------------------------------------------------------------------------
	FGameplayTagContainer ItemTag;
	if (Item->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
	{
		ItemTag = IItemInterface::Execute_GetItemTag(Item);
	}
	else
	{
		return;
	}

	// ASC에 부여된 어빌리티의 핸들 각각----------------------------------------------------------------------------
	TArray<FGameplayAbilitySpecHandle> Handles;
	AbilitySystemComponent->GetAllAbilities(Handles);

	for (const FGameplayAbilitySpecHandle& Handle : Handles)
	{
		// 어빌리티 핸들을 통해 어빌리티 클래스 얻기-----------------------------------------------------------------------------
		bool bIsInstance = false;
		const UGameplayAbility* AbilityObj =
			UAbilitySystemBlueprintLibrary::GetGameplayAbilityFromSpecHandle(AbilitySystemComponent, Handle, bIsInstance);
		if (!AbilityObj)
		{
			continue;
		}

		UClass* AbilityClass = AbilityObj->GetClass();

		// 장착 어빌리티인지 체크---------------------------------------------------------------------------------------------------
		if (!AbilityClass->IsChildOf(UGA_Equip::StaticClass()))
		{
			continue; // 장착 GA가 아니면 스킵
		}

		// GA_Equip CDO에서 ItemClass 뽑기--------------------------------------------------------------
		UGA_Equip* EquipCDO = Cast<UGA_Equip>(AbilityClass->GetDefaultObject());
		if (!EquipCDO || !EquipCDO->GetItemClass())
		{
			continue;
		}

		AItemBase* ItemCDO = EquipCDO->GetItemClass()->GetDefaultObject<AItemBase>();
		if (!ItemCDO)
		{
			continue;
		}

		FGameplayTagContainer AbilityItemTag;
		if (ItemCDO->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
		{
			AbilityItemTag = IItemInterface::Execute_GetItemTag(ItemCDO);
		}
		else
		{
			return;
		}

		// 장착 어빌리티와 매칭된 아이템의 태그가 일치하는 지 체크------------------------------------------------------
		if (AbilityItemTag == ItemTag)
		{
			// 어빌리티 핸들을 통해 ASC에서 해당 어빌리티 제거---------------------------------------------
			AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

void APandorasCharacterBase::OnDestroyItem(AActor* Item)
{
	// 아이템 유효성 체크----------------------------------------------------------------
	if (!IsValid(Item))
	{
		return;
	}

	// 플레이어의 캐릭터인지 체크-------------------------------------------------------------
	const bool bIsPlayer = ICharacterInterface::Execute_IsPlayer(this);

	// 아이템 장착 어빌리티 제거-------------------------------------------------------------
	if (bIsPlayer)
	{
		ClearItemAbilities(Item);
	}

	// 아이템 태그와 매칭되는 GE 삭제------------------------------------------------------------
	FGameplayTagContainer ItemTag;
	ItemTag = IItemInterface::Execute_GetItemTag(Item);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(ItemTag);
	}

	// 아이템 삭제------------------------------------------------------------------------
	Item->Destroy();
}

void APandorasCharacterBase::DestroyItem_Implementation(EItem ItemType)
{
	DestroyItem_Server(ItemType);
}

bool APandorasCharacterBase::DestroyItem_Server_Validate(EItem ItemType)
{
	return true;
}

void APandorasCharacterBase::DestroyItem_Server_Implementation(EItem ItemType)
{
	DestroyItem_Multicast(ItemType);
}

void APandorasCharacterBase::DestroyItem_Multicast_Implementation(EItem ItemType)
{
	// 무기 종류에 따라 삭제
	switch (ItemType)
	{
	case EItem::Sword:
	case EItem::GreatSword:
		OnDestroyItem(CurrentWeapon);
		break;

	case EItem::Helmet:
		OnDestroyItem(CurrentHelmet);
		break;

	case EItem::Armor:
		OnDestroyItem(CurrentArmor);
		break;

	case EItem::Glove:
		OnDestroyItem(CurrentGlove);
		break;

	case EItem::Shoes:
		OnDestroyItem(CurrentShoes);
		break;

	case EItem::UnArmed:
	case EItem::Bow:
	default:
		break;
	}
}

void APandorasCharacterBase::SetMovementMode_Implementation(ECustomMovementMode MovementMode)
{
	SetMovementMode_Server(MovementMode);
}

bool APandorasCharacterBase::SetMovementMode_Server_Validate(ECustomMovementMode NewMovementMode)
{
	return true;
}

void APandorasCharacterBase::SetMovementMode_Server_Implementation(ECustomMovementMode NewMovementMode)
{
	// 현재 이동 모드 저장---------------------------------------------------------------------------------
	CurrentMovementMode = NewMovementMode;

	// 이동 속력 계산하여 최대 속력 갱신--------------------------------------------------------------------
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = CalculateMovementSpeed();
	}

	// AnimInstance의 이동모드 갱신-----------------------------------------------------------------------------
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
		{
			ICharacterAnimationInterface::Execute_UpdateMovementMode(AnimInst, CurrentMovementMode);
		}
	}
}

float APandorasCharacterBase::CalculateMovementSpeed_Implementation()
{
	switch (CurrentMovementMode)
	{
	case ECustomMovementMode::Crouch:  return 200.f;
	case ECustomMovementMode::Walk:    return 250.f;
	case ECustomMovementMode::Run:     return 500.f;
	case ECustomMovementMode::Sprint:  return 650.f;
	default:                           return 250.f;
	}
}

void APandorasCharacterBase::OnRep_CurrentMovementMode_Implementation()
{
	// 이동 속력 계산하여 최대 속력 갱신------------------------------------------------------------------------
	float NewMaxWalkSpeed = 250.f;
	switch (CurrentMovementMode)
	{
	case ECustomMovementMode::Crouch:  NewMaxWalkSpeed = 200.f; break;
	case ECustomMovementMode::Walk:    NewMaxWalkSpeed = 250.f; break;
	case ECustomMovementMode::Run:     NewMaxWalkSpeed = 500.f; break;
	case ECustomMovementMode::Sprint:  NewMaxWalkSpeed = 650.f; break;
	default:                           NewMaxWalkSpeed = 250.f; break;
	}

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->MaxWalkSpeed = NewMaxWalkSpeed;
	}

	// AnimInstance의 이동모드 갱신-----------------------------------------------------------------------------
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
		{
			if (AnimInst->GetClass()->ImplementsInterface(UCharacterAnimationInterface::StaticClass()))
			{
				ICharacterAnimationInterface::Execute_UpdateMovementMode(AnimInst, CurrentMovementMode);
			}
		}
	}
}

void APandorasCharacterBase::ToggleWalk_Implementation()
{
	// 현재 이동모드 체크
	switch (CurrentMovementMode)
	{
	case ECustomMovementMode::Walk:
		// 걷기 -> 달리기
		ICharacterInterface::Execute_SetMovementMode(this, ECustomMovementMode::Run);
		break;

	case ECustomMovementMode::Crouch:
	case ECustomMovementMode::Run:
	case ECustomMovementMode::Sprint:
	default:
		// 다른 이동모드 -> 걷기
		ICharacterInterface::Execute_SetMovementMode(this, ECustomMovementMode::Walk);
		break;
	}
}

void APandorasCharacterBase::ToggleCrouch_Implementation()
{
	// 현재 이동모드 체크
	switch (CurrentMovementMode)
	{
	case ECustomMovementMode::Crouch:
		// 천천히 걷기 -> 걷기
		ICharacterInterface::Execute_SetMovementMode(this, ECustomMovementMode::Walk);
		break;

	case ECustomMovementMode::Walk:
	case ECustomMovementMode::Run:
	case ECustomMovementMode::Sprint:
	default:
		// 다른 이동모드 -> 천천히 걷기
		ICharacterInterface::Execute_SetMovementMode(this, ECustomMovementMode::Crouch);
		break;
	}
}

void APandorasCharacterBase::Sprint_Implementation()
{
	// 현재 이동모드 체크
	switch (CurrentMovementMode)
	{
	case ECustomMovementMode::Sprint:
		// 전력질주 -> 달리기
		ICharacterInterface::Execute_SetMovementMode(this, ECustomMovementMode::Run);
		break;

	case ECustomMovementMode::Crouch:
	case ECustomMovementMode::Run:
	case ECustomMovementMode::Walk:
	default:
		// 다른 이동모드 -> 전력질주
		ICharacterInterface::Execute_SetMovementMode(this, ECustomMovementMode::Sprint);
		break;
	}
}

void APandorasCharacterBase::Attack_Implementation()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	const FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Event.Attack"));
	FGameplayTagContainer AttackTags; 
	AttackTags.AddTag(AttackTag);

	const FGameplayTag EquipTag = FGameplayTag::RequestGameplayTag(TEXT("Input.Weapon.Equip"));
	FGameplayTagContainer EquipTags; 
	EquipTags.AddTag(EquipTag);

	// 공격 실행하고 실행이 유효한지 체크---------------------------------------------------------------------------------------------------------------
	const bool bAttackActivated = AbilitySystemComponent->TryActivateAbilitiesByTag(AttackTags, /*bAllowRemoteActivation=*/true);

	if (bAttackActivated)
	{
		// 차징 허용 GE 적용
		if (EnableChargeAttackEffectClass)
		{
			ApplyGameplayEffect_Server(EnableChargeAttackEffectClass);
		}
		return;
	}

	// 장착 실행하고 실행이 유효한지 체크---------------------------------------------------------------------------------------------------------------
	const bool bEquipActivated = AbilitySystemComponent->TryActivateAbilitiesByTag(EquipTags, /*bAllowRemoteActivation=*/true);
	if (bEquipActivated)
	{
		// 차징 허용 GE 적용
		if (EnableChargeAttackEffectClass)
		{
			ApplyGameplayEffect_Server(EnableChargeAttackEffectClass);
		}

		// 0.5초 후 공격 실행------------------------------------------------------------------------------------------------------------------------
		FGameplayTagContainer AttackTagsCopy = AttackTags;
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(
			Handle,
			[this, AttackTagsCopy]()
			{
				if (AbilitySystemComponent)
				{
					AbilitySystemComponent->TryActivateAbilitiesByTag(AttackTagsCopy, /*bAllowRemoteActivation=*/true);
				}
			},
			0.5f,
			false
		);
	}
}

bool APandorasCharacterBase::ClearGameplayEffect_Server_Validate(FGameplayTagContainer GameplayTags)
{
	return true;
}

void APandorasCharacterBase::ClearGameplayEffect_Server_Implementation(FGameplayTagContainer GameplayTags)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(GameplayTags);
}

void APandorasCharacterBase::StopAttack_Implementation()
{
	// 차징 허용 GE 제거-----------------------------------------------------------------------------------------------------------------
	const FGameplayTag ChargeAttackEnabledTag = FGameplayTag::RequestGameplayTag(TEXT("Character.State.ChargeAttackEnabled"));
	FGameplayTagContainer ChargeAttackEnabledTags; 
	ChargeAttackEnabledTags.AddTag(ChargeAttackEnabledTag);
	ClearGameplayEffect_Server(ChargeAttackEnabledTags);
}

void APandorasCharacterBase::ChargeAttack_Implementation()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// 공격 차징 중인지 체크-----------------------------------------------------------------------------------------------------------
	const FGameplayTag ChargeEnabledTag = FGameplayTag::RequestGameplayTag(TEXT("Character.State.ChargeAttackEnabled"));
	FGameplayTagContainer ChargeEnabledContainer;
	ChargeEnabledContainer.AddTag(ChargeEnabledTag);

	const bool bIsChargingAllowed =
		AbilitySystemComponent->HasAnyMatchingGameplayTags(ChargeEnabledContainer);

	if (!bIsChargingAllowed)
	{
		return;
	}

	// 강공격스킬 어빌리티 실행을 성공했는 지 체크----------------------------------------------------------------------------------------
	const FGameplayTag HeavyAttackTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Upgrade.Combat.HeavyAttack"));
	FGameplayTagContainer HeavyAttackTags;
	HeavyAttackTags.AddTag(HeavyAttackTag);

	const bool bActivated = AbilitySystemComponent->TryActivateAbilitiesByTag(HeavyAttackTags);
	if (!bActivated)
	{
		return;
	}

	// 애니메이션 속도 늦추기------------------------------------------------------------------------------------------------------
	if (USkeletalMeshComponent* SkelMesh = GetMesh())
	{
		SkelMesh->GlobalAnimRateScale = 0.05f;
	}

	// 0.2초 후 무기가 차징됨--------------------------------------------------------------------------------------------
	FTimerHandle ChargeTimerHandle;
	GetWorldTimerManager().SetTimer(
		ChargeTimerHandle,
		[this]()
		{
			UObject* WeaponObj = nullptr;
			if (GetClass()->ImplementsInterface(UItemWielderInterface::StaticClass()))
			{
				WeaponObj = IItemWielderInterface::Execute_GetWeapon(this);
			}
			if (WeaponObj && WeaponObj->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
			{
				IItemInterface::Execute_ChargeWeapon(WeaponObj);
			}

			// 또 0.2초 후 애니메이션 속도 원상복구 --------------------------------------------------------------------------------
			FTimerHandle RestoreTimerHandle;
			GetWorldTimerManager().SetTimer(
				RestoreTimerHandle,
				[this]()
				{
					if (USkeletalMeshComponent* Skel = GetMesh())
					{
						Skel->GlobalAnimRateScale = 1.0f;
					}
				},
				0.2f, false);
		},
		0.2f, false);
}

void APandorasCharacterBase::NotifyAttack_Implementation(bool IsNonBlockable)
{
	// 한 번 실행되고 2초동안 실행막기---------------------------------------------------------------
	if (!bNotifyAttack_DoOnceClosed)
	{
		bNotifyAttack_DoOnceClosed = true; // DoOnce 닫기

		// 공격이 블로킹 가능한 지에 따라 매칭되는 이펙트를 손에 부착---------------------------------------------------------------
		const FName TagName = IsNonBlockable
			? TEXT("GameplayCue.NotifyAttack.NonBlockable")
			: TEXT("GameplayCue.NotifyAttack.Blockable");
		const FGameplayTag SelectedTag = FGameplayTag::RequestGameplayTag(TagName);

		FGameplayCueParameters Params;
		Params.Location = GetActorLocation();
		Params.Normal = FVector::ZeroVector;
		Params.GameplayEffectLevel = 1;
		Params.AbilityLevel = 1;

		ICharacterGameAbilityInterface::Execute_ExecuteGameplayCue_Replicated(this, this, SelectedTag, Params);
	}

	// 2초동안 실행막기
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(
		Handle,
		[this]()
		{
			bNotifyAttack_DoOnceClosed = false;
		},
		2.0f, false);
}

void APandorasCharacterBase::FinishAttack_Implementation()
{
	if (AbilitySystemComponent)
	{
		// 피니셔 공격 어빌리티 실행-----------------------------------------------------------------------------
		FGameplayTagContainer FinishAttackTags;
		FinishAttackTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.FinishAttack")));
		AbilitySystemComponent->TryActivateAbilitiesByTag(FinishAttackTags, /*bAllowRemoteActivation=*/true);

		// 테이크다운 공격 어빌리티 실행--------------------------------------------------------------------------------
		FGameplayTagContainer TakeDownTags;
		TakeDownTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.TakeDown")));
		AbilitySystemComponent->TryActivateAbilitiesByTag(TakeDownTags, /*bAllowRemoteActivation=*/true);
	}
}

void APandorasCharacterBase::AreaOfEffect_Implementation()
{
	// 범위 공격 어빌리티 실행--------------------------------------------------------------------------------
	FGameplayTagContainer AOETags;
	AOETags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.AOE.Radial")));
	AbilitySystemComponent->TryActivateAbilitiesByTag(AOETags, /*bAllowRemoteActivation=*/true);
}

bool APandorasCharacterBase::ApplyGameplayEffect_Server_Validate(TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	return true;
}

void APandorasCharacterBase::ApplyGameplayEffect_Server_Implementation(TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	ApplyGameplayEffect_Server(GameplayEffectClass);
}

void APandorasCharacterBase::ApplyGameplayEffect_Replicate_Implementation(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (!AbilitySystemComponent || !GameplayEffect)
	{
		return;
	}

	const FGameplayEffectContextHandle EmptyContext;
	AbilitySystemComponent->BP_ApplyGameplayEffectToSelf(GameplayEffect, /*Level=*/0.0f, /*EffectContext=*/EmptyContext);
}

void APandorasCharacterBase::LockOn_Implementation()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	const FName TagName = LockTarget != nullptr
		? TEXT("Character.Event.DeactivateLockOn")
		: TEXT("Character.Event.LockOn");

	// 락온 또는 락온끄기 어빌리티 실행
	FGameplayTagContainer LockTags;
	LockTags.AddTag(FGameplayTag::RequestGameplayTag(TagName));
	AbilitySystemComponent->TryActivateAbilitiesByTag(LockTags, /*bAllowRemoteActivation=*/true);
}

void APandorasCharacterBase::SetLockTarget_Implementation(AActor* InLockTarget)
{
	// 새로운 락타겟 저장------------------------------------------------------------
	LockTarget = InLockTarget;

	// 이동 방향으로 캐릭터 회전을 자동으로 맞추지 않기-----------------------------------------
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->bOrientRotationToMovement = false;
	}
}

void APandorasCharacterBase::ClearLockTarget_Implementation()
{
	// 락타겟 해제---------------------------------------------------------------------
	LockTarget = nullptr;

	// 이동 방향으로 캐릭터 회전을 자동으로 맞추기-------------------------------------------------
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->bOrientRotationToMovement = true;
	}
}

void APandorasCharacterBase::FocusOnEnemy_Implementation()
{
	// LockTarget 유효?
	if (!LockTarget) return;

	// 회피 중인지 체크--------------------------------------------------------------------------
	if (AbilitySystemComponent &&
		AbilitySystemComponent->HasMatchingGameplayTag(
			FGameplayTag::RequestGameplayTag(FName("Character.State.Evading"))))
	{
		return; // 회피 중이면 아무 것도 하지 않음
	}

	const FVector MyLoc = GetActorLocation();
	const FVector TargetLoc = LockTarget->GetActorLocation();

	const FRotator CurRot = GetActorRotation();
	const FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(MyLoc, TargetLoc);

	const float   DeltaSeconds = GetWorld() ? GetWorld()->GetDeltaSeconds() : UGameplayStatics::GetWorldDeltaSeconds(this);
	const FVector Vel = GetVelocity();
	const float   Speed = Vel.Size();

	// 속도가 5보다 큰지 체크---------------------------------------------------------------------------------------------------
	if (Speed > SpeedThreshold)
	{
		// 캐릭터의 정면이 락타겟을 향하도록 부드럽게 회전-----------------------------------------------------------------
		const FRotator Interped = UKismetMathLibrary::RInterpTo(CurRot, LookRot, DeltaSeconds, 10.f);
		SetActorRotation(FRotator(0.f, Interped.Yaw, 0.f));
		return;
	}

	// 락 타겟이 캐릭터가 바라보고 있는 방향에서 85보다 더 각도 차이가 나는 지 체크-----------------------------------------------------------
	const FRotator NormDelta = UKismetMathLibrary::NormalizedDeltaRotator(CurRot, LookRot);
	const double   AbsDeltaYaw = FMath::Abs((double)NormDelta.Yaw);

	if (AbsDeltaYaw > (double)AngleThresholdDeg)
	{
		if (bTurningMontagePlaying_DoOnceClosed) return; // DoOnce: 이미 재생 중이면 무시

		// 좌우 방향 전환 애니메이션 재생-------------------------------------------------------------------------------------------------------
		UAnimMontage* MontageToPlay = (NormDelta.Yaw > 0.f) ? TurnLeft90Montage : TurnRight90Montage;
		if (USkeletalMeshComponent* MeshComp = GetMesh())
		{
			if (UAnimInstance* Anim = MeshComp->GetAnimInstance())
			{
				Anim->StopAllMontages(0.f);

				if (MontageToPlay)
				{
					bTurningMontagePlaying_DoOnceClosed = true;

					FOnMontageBlendingOutStarted BlendOut;
					BlendOut.BindWeakLambda(this, [this](UAnimMontage* Montage, bool bInterrupted)
						{
							bTurningMontagePlaying_DoOnceClosed = false;
						});
					Anim->Montage_SetBlendingOutDelegate(BlendOut, MontageToPlay);

					FOnMontageEnded Ended;
					Ended.BindWeakLambda(this, [this](UAnimMontage* Montage, bool bInterrupted)
						{
							bTurningMontagePlaying_DoOnceClosed = false;
						});
					Anim->Montage_SetEndDelegate(Ended, MontageToPlay);

					Anim->Montage_Play(MontageToPlay, /*PlayRate*/1.f);
				}
			}
		}
	}
}

ECustomMovementMode APandorasCharacterBase::GetMovementMode_Implementation()
{
	return CurrentMovementMode;
}

void APandorasCharacterBase::UpdateHealth_Implementation(float NewHealth)
{
	// 체력값이 0 이하인지 체크----------------------------------------------------------------------------------------
	if (NewHealth <= 0.0f)
	{
		if (AbilitySystemComponent)
		{
			// 사망 어빌리티 실행----------------------------------------------------------------------------------------------
			FGameplayTagContainer DeathTags;
			const FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag(TEXT("Character.Event.Death"));
			DeathTags.AddTag(DeathTag);

			AbilitySystemComponent->TryActivateAbilitiesByTag(DeathTags, /*bAllowRemoteActivation*/ true);
		}
	}

	// 로컬이자 플레이어 캐릭터인지 체크--------------------------------------------------------------------------------------
	if (IsPlayerControlled() && IsLocallyControlled())
	{
		// HUD의 체력바 갱신 (현재체력/최대체력)----------------------------------------------------------------
		UBaseActorAttributes* AttrSet = nullptr;
		if (GetClass()->ImplementsInterface(UCharacterInterface::StaticClass()))
		{
			AttrSet = ICharacterInterface::Execute_GetBaseActorAttribute(this);
		}

		if (AttrSet)
		{
			const float MaxHealth = AttrSet->GetMaxHealth(); // ATTRIBUTE_ACCESSORS 사용
			const float Ratio = (MaxHealth > 0.f) ? (NewHealth / MaxHealth) : 0.f;

			if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
			{
				if (AHUD* HUD = PC->GetHUD())
				{
					if (HUD->GetClass()->ImplementsInterface(UHudInterface::StaticClass()))
					{
						IHudInterface::Execute_UpdateHealth(HUD, Ratio);
					}
				}
			}
		}
	}
}

void APandorasCharacterBase::UpdateStamina_Implementation(float NewStamina)
{
	// 로컬 & 플레이어 캐릭터인지 체크---------------------------------------------------------------------------
	if (IsPlayerControlled() && IsLocallyControlled())
	{
		// HUD의 스테미나 바 갱신 (백분위)--------------------------------------------------------------------
		const float StaminaPercent = NewStamina / 100.f;

		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			if (AHUD* HUD = PC->GetHUD())
			{
				if (HUD->GetClass()->ImplementsInterface(UHudInterface::StaticClass()))
				{
					IHudInterface::Execute_UpdateStamina(HUD, StaminaPercent);
				}
			}
		}
	}
}


void APandorasCharacterBase::UpdateXPPoints_Implementation(const float NewXPPoints)
{
	// 플레이어인지 체크----------------------------------------------------------------------
	if (IsPlayerControlled())
	{
		// 10초동안 레벨 UI를 HUD에 표시------------------------------------------------------
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			if (AHUD* HUD = PC->GetHUD())
			{
				if (HUD->GetClass()->ImplementsInterface(UHudInterface::StaticClass()))
				{
					IHudInterface::Execute_ShowCharacterLevel(HUD, 10.f);
				}
			}
		}

		// 레벨 업 어빌리티 실행--------------------------------------------------------------------
		if (AbilitySystemComponent)
		{
			if (LevelUpAbilityClass)
			{
				AbilitySystemComponent->K2_GiveAbilityAndActivateOnce(LevelUpAbilityClass, /*Level*/0, /*InputID*/-1);
			}
		}
	}
}

void APandorasCharacterBase::UpdateMaxHealth_Implementation(const float NewMaxHealth)
{
	OnMaxHealthUpdated.Broadcast();
}
