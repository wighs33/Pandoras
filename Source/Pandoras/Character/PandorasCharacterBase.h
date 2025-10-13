// 판도라즈 전용 캐릭터의 베이스

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Common/Structs.h"
#include "Common/Enums.h"
#include "AttributeSet/BaseActorAttributes.h"
#include "GameplayTagContainer.h"

#include "Interface/ItemWielderInterface.h"
#include "Interface/CharacterInterface.h"
#include "Interface/CharacterGameAbilityInterface.h"
#include "Interface/FactionsInterface.h"
#include "Interface/GASInterface.h"

#include "PandorasCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UInputComponent;
class UAbilitySystemComponent;
class UCharacterTrajectoryComponent;
class UGameplayAbility;
class USoundBase;
class AItemBase;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxHealthUpdated);

UCLASS(config=Game)
class APandorasCharacterBase : 
	public ACharacter, 
	// 아이템 사용 인터페이스
	public IItemWielderInterface, 
	// 캐릭터 관련 인터페이스
	public ICharacterInterface,
	// 캐릭터의 어빌리티 관련 인터페이스
	public ICharacterGameAbilityInterface,
	// 파벌 인터페이스
	public IFactionsInterface,
	// 어빌리티 시스템 인터페이스
	public IGASInterface
{
	GENERATED_BODY()

public:
	// 생성자
	APandorasCharacterBase();
			
protected:
	// 컴포넌트 생성 직후 호출
	virtual void PostInitializeComponents() override;

	// 게임 플레이 시작
	virtual void BeginPlay() override;

	// 변수 복제를 위해 반드시 GetLifetimeReplicatedProps 를 오버라이드
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 컨트롤러의 설정이 변경될 때 호출
	virtual void NotifyControllerChanged() override;

	// 폰이 컨트롤러에 소유될 때 호출
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	// 스프링암 리턴
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// 카메라 리턴
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

// 액션
protected:
	// 이동
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void Move(const FInputActionValue& Value);

	// 시선
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void Look(const FInputActionValue& Value);

	// 공격
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void Attack();

	// 공격 중지
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void StopAttack();

	// 락온
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void LockOn();

	// 블로킹
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void Block();

	// 블로킹 중지
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void StopBlocking();

	// 피니셔 공격
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void FinishAttack();

	// 회피
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void Evade();

	// 걷기 전환
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void ToggleWalk();

	// 쪼그려 앉기 전환
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void ToggleCrouch();

	// 전력 질주 전환
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void Sprint();

	// 범위 공격
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void AreaOfEffect();

// OnRep_X: 값 변경 시 클라에서 호출
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void OnRep_Dead();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void OnRep_MontageData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void OnRep_WeaponType();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void OnRep_CurrentMovementMode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void OnRep_Armor();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void OnRep_Glove();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void OnRep_Shoes();

// RPC
protected:
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void DestroyItem_Server(EItem ItemType);
    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "C++")
    void DestroyItem_Multicast(EItem ItemType);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void ApplyGameplayEffect_Server(TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void ClearGameplayEffect_Server(FGameplayTagContainer GameplayTags);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void SetMovementMode_Server(ECustomMovementMode NewMovementMode);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void GiveAndActivateAbility_Server(TSubclassOf<UGameplayAbility> Ability);
	void GiveAndActivateAbility_Server_Implementation(TSubclassOf<UGameplayAbility> Ability);
	bool GiveAndActivateAbility_Server_Validate(TSubclassOf<UGameplayAbility> Ability);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_GiveAndActivateAbility_Server(TSubclassOf<UGameplayAbility> Ability);	

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void OnlyGiveAbility_Server(TSubclassOf<UGameplayAbility> Ability);
	void OnlyGiveAbility_Server_Implementation(TSubclassOf<UGameplayAbility> Ability);
	bool OnlyGiveAbility_Server_Validate(TSubclassOf<UGameplayAbility> Ability);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_OnlyGiveAbility_Server(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "C++")
	void SendGameplayEvent_Multicast(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void ExecuteGameplayCue_Server(AActor* TargetActor, FGameplayTag GamplayCueTag, const FGameplayCueParameters Parameters);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "C++")
	void ExecuteGameplayCue_Multicast(AActor* TargetActor, FGameplayTag GamplayCueTag, const FGameplayCueParameters Parameters);

// 어트리뷰트
protected:
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void XPPointsChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintNativeEvent, Category = "Base Actor Attributes")
	void UpdateHealth(const float NewHealth);

	UFUNCTION(BlueprintNativeEvent, Category = "Base Actor Attributes")
	void UpdateStamina(const float NewStamina);

	UFUNCTION(BlueprintNativeEvent, Category = "Base Actor Attributes")
	void UpdateXPPoints(const float NewXPPoints);

	UFUNCTION(BlueprintNativeEvent, Category = "Base Actor Attributes")
	void UpdateMaxHealth(const float NewMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Base Actor Attributes")
	void CallOnMaxHealthUpdated() { OnMaxHealthUpdated.Broadcast(); }

	UFUNCTION(BlueprintCallable, Category = "Base Actor Attributes")
	void LoadAttributes(TMap<FGameplayAttribute, float> SavedAttributesMap);

// IItemWielderInterface
protected:
	virtual void EquipItem_Implementation(AItemBase* Item) override;
	virtual void UnequipItem_Implementation() override;
	virtual void SetAttackState_Implementation(EAttackState InAttackState) override;
	virtual void AddItemToPlayerState_Implementation(TSubclassOf<UGA_Equip> itemAbilityClass) override;
	virtual void DestroyItem_Implementation(EItem ItemType) override;
	virtual void ChargeAttack_Implementation() override;
	virtual void NotifyAttack_Implementation(bool IsNonBlockable) override;

// ICharacterInterface
protected:
	virtual void NotifyFootstep_Implementation() override;
	virtual void SlowDown_Implementation(float Rate = 0.2f, float Duration = 0.4f) override;
	virtual void Die_Implementation() override;
	virtual void PlayMontageReplicated_Implementation(UAnimMontage* AnimMontage, float InPlayRate = 1.0, FName StartSectionName = TEXT("None")) override;
	virtual void SetMovementMode_Implementation(ECustomMovementMode MovementMode) override;
	// 락 타겟 저장
	virtual void SetLockTarget_Implementation(AActor* LockTarget) override;
	// 락 타겟 비우기
	virtual void ClearLockTarget_Implementation() override;
	virtual ECustomMovementMode GetMovementMode_Implementation() override;

// ICharacterGameAbilityInterface
protected:
	virtual void SendGameplayEvent_Replicated_Implementation(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload) override;
	virtual void ExecuteGameplayCue_Replicated_Implementation(AActor* TargetActor, FGameplayTag GamplayCueTag, const FGameplayCueParameters Parameters) override;

// IGASInterface
protected:
	virtual void ApplyGameplayEffect_Replicate_Implementation(TSubclassOf<UGameplayEffect> GameplayEffect) override;

// 미분류
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void UpdateWeapon(EItem ItemType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void GiveDefaultAbilities();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void FocusOnEnemy();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void AddDefaultWeaponAbilities();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "C++")
	float CalculateMovementSpeed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void ClearItemAbilities(AActor* Item);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void SaveCharacterData();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void LoadCharacterData();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void RefreshUI();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void AttachToSocket(AActor* Target, EItem Index);

	UFUNCTION()
	void AttachWeaponToRightHand_Deferred();

	UFUNCTION()
	void InitItemLeadPose(AItemBase* Item);

	UFUNCTION()
	void DestroyAIController();

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnDestroyItem(AActor* Item);

// 컴포넌트
protected:
	// 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// 게임 어빌리티 시스템 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 추적
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UCharacterTrajectoryComponent> CharacterTrajectory;

// 입력 액션
protected:
	// 매핑 컨텍스트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// 점프 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// 이동 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// 시선 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// 공격 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;

	// 락온 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LockOnAction;

	// 블로킹 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BlockAction;

	// 피니셔 공격 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FinishAttackAction;

	// 회피 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EvadeAction;

	// 걷기 전환 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ToggleWalkAction;

	// 쪼그려 앉기 전환 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ToggleCrouchAction;

	// 전력 질주 전환 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;

	// 범위 공격 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AreaOfEffectAction;

// 그 외 변수
protected:
	// 기본 어트리뷰트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<const UBaseActorAttributes> BaseActorAttributes;

	// 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "C++")
	TObjectPtr<AItemBase> CurrentWeapon;

	// 투구
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "C++")
	TObjectPtr<AItemBase> CurrentHelmet;

	// 갑옷
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "C++")
	TObjectPtr<AItemBase> CurrentArmor;

	// 장갑
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Glove, Category = "C++")
	TObjectPtr<AItemBase> CurrentGlove;

	// 신발
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Shoes, Category = "C++")
	TObjectPtr<AItemBase> CurrentShoes;

	// 사망상태
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Dead, Category = "C++")
	bool bDead;

	// 몽타주 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MontageData, Category = "C++")
	FMontage MontageData;

	// 기본시작 어빌리티
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// 락 타겟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++")
	AActor* LockTarget;

	// 기본시작 무기의 장착 어빌리티
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> DefaultWeaponAbilities;

	// 파벌 아이디
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 FactionId;

	// 공격 중 검 좌우 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	EAttackState AttackState;

	// 현재 이동 모드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentMovementMode, Category = "C++")
	ECustomMovementMode CurrentMovementMode = ECustomMovementMode::Run;

	// 무기 타입
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponType, Category = "C++")
	EItem WeaponType;

	// 게임 저장 파일명
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	FString SaveSlot = TEXT("SV_Character");

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> EquipMontage_Sword;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> EquipMontage_GreatSword;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> UnequipMontage_Sword;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> UnequipMontage_GreatSword;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<USoundBase> FootstepCue = nullptr;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> EnableChargeAttackEffectClass;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> TurnLeft90Montage;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> TurnRight90Montage;

	// 에디터에서 애셋 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayAbility> LevelUpAbilityClass;

	UPROPERTY()
	bool bNotifyAttack_DoOnceClosed = false;

	UPROPERTY()
	bool bTurningMontagePlaying_DoOnceClosed = false;

	FTimerHandle TimerHandle_AttachWeapon;
	FTimerHandle TimerHandle_RefreshUI;

	UPROPERTY()
	float SpeedThreshold = 5.f;

	UPROPERTY()
	float AngleThresholdDeg = 85.f;  

// 델리게이트
protected:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnMaxHealthUpdated OnMaxHealthUpdated;
};

