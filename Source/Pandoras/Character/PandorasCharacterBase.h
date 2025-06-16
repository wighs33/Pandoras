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
	void Move(const FInputActionValue& Value);

	// 시선
	void Look(const FInputActionValue& Value);

	// 공격
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Attack();

	// 공격 중지
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void StopAttack();

	// 락온
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void LockOn();

	// 블로킹
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Block();

	// 블로킹 중지
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void StopBlocking();

	// 피니셔 공격
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void FinishAttack();

	// 회피
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Evade();

	// 걷기 전환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ToggleWalk();

	// 쪼그려 앉기 전환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ToggleCrouch();

	// 전력 질주 전환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Sprint();

	// 전력 질주 전환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void QuickSave();

// OnRep_X: 값 변경 시 클라에서 호출
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_Dead();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_MontageData();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_WeaponType();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_CurrentMovementMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_Armor();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_Glove();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_Shoes();

// RPC
protected:
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void DestroyItem_Server(EItem ItemType);
	// 게임 완성 전엔 임시로 블루르린트에서 진행
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_DestroyItem_Server(EItem ItemType);
	// 선언만 .h에 정의는 무조건 .cpp에서 진행
	bool DestroyItem_Server_Validate(EItem ItemType);
	void DestroyItem_Server_Implementation(EItem ItemType);

    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "C++")
    void DestroyItem_Multicast(EItem ItemType);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_DestroyItem_Multicast(EItem ItemType);
	void DestroyItem_Multicast_Implementation(EItem ItemType);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void ApplyGameplayEffect_Server(TSubclassOf<UGameplayEffect> GameplayEffectClass);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_ApplyGameplayEffect_Server(TSubclassOf<UGameplayEffect> GameplayEffectClass);
	bool ApplyGameplayEffect_Server_Validate(TSubclassOf<UGameplayEffect> GameplayEffectClass);
	void ApplyGameplayEffect_Server_Implementation(TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void ClearGameplayEffect_Server(FGameplayTagContainer GameplayTags);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_ClearGameplayEffect_Server(FGameplayTagContainer GameplayTags);
	bool ClearGameplayEffect_Server_Validate(FGameplayTagContainer GameplayTags);
	void ClearGameplayEffect_Server_Implementation(FGameplayTagContainer GameplayTags);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void SetMovementMode_Server(ECustomMovementMode NewMovementMode);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_SetMovementMode_Server(ECustomMovementMode NewMovementMode);
	bool SetMovementMode_Server_Validate(ECustomMovementMode NewMovementMode);
	void SetMovementMode_Server_Implementation(ECustomMovementMode NewMovementMode);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void GiveAndActivateAbility_Server(TSubclassOf<UGameplayAbility> Ability);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_GiveAndActivateAbility_Server(TSubclassOf<UGameplayAbility> Ability);	
	bool GiveAndActivateAbility_Server_Validate(TSubclassOf<UGameplayAbility> Ability);
	void GiveAndActivateAbility_Server_Implementation(TSubclassOf<UGameplayAbility> Ability);

// 어트리뷰트
protected:
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void XPPointsChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Actor Attributes")
	void UpdateHealth(const float NewHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Actor Attributes")
	void UpdateStamina(const float NewStamina);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Actor Attributes")
	void UpdateXPPoints(const float NewXPPoints);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Actor Attributes")
	void UpdateMaxHealth(const float NewXPPoints);

	UFUNCTION(BlueprintCallable, Category = "Base Actor Attributes")
	void CallOnMaxHealthUpdated() { OnMaxHealthUpdated.Broadcast(); }

// 미분류
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void UpdateWeapon(EItem ItemType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void GiveDefaultAbilities();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void FocusOnEnemy();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void AddDefaultWeaponAbilities();

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "C++")
	float CalculateMovementSpeed();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ClearItemAbilities(AActor* Item);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void SaveCharacterData();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void LoadCharacterData();

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

	// 전력 질주 전환 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* QuickSaveAction;

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

	// 원래 이동 모드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	ECustomMovementMode OriginalMovementMode = ECustomMovementMode::Crouch;

	// 무기 타입
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponType, Category = "C++")
	EItem WeaponType;

	// 게임 저장 파일명
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	FString SaveSlot = TEXT("SV_Character");

// 델리게이트
protected:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnMaxHealthUpdated OnMaxHealthUpdated;
};

