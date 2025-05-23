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
	public IFactionsInterface
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

// OnRep_X: 값 변경 시 클라에서 호출
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_Dead();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_MontageData();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_WeaponType();

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

	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable, Category = "C++")
	void ApplyGameplayEffect_Server(TSubclassOf<UGameplayEffect> GameplayEffectClass);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_ApplyGameplayEffect_Server(TSubclassOf<UGameplayEffect> GameplayEffectClass);
	bool ApplyGameplayEffect_Server_Validate(TSubclassOf<UGameplayEffect> GameplayEffectClass);
	void ApplyGameplayEffect_Server_Implementation(TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable, Category = "C++")
	void ClearGameplayEffect_Server(FGameplayTagContainer GameplayTags);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_ClearGameplayEffect_Server(FGameplayTagContainer GameplayTags);
	bool ClearGameplayEffect_Server_Validate(FGameplayTagContainer GameplayTags);
	void ClearGameplayEffect_Server_Implementation(FGameplayTagContainer GameplayTags);

// 어트리뷰트
protected:
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Actor Attributes")
	void UpdateHealth(const float NewHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Actor Attributes")
	void UpdateStamina(const float NewStamina);

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

protected:
	// 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
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

	// 기본 어트리뷰트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<const UBaseActorAttributes> BaseActorAttributes;

	// 게임 어빌리티 시스템 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 게임 어빌리티 시스템 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UCharacterTrajectoryComponent> CharacterTrajectory;

	// 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "C++")
	TObjectPtr<AItemBase> CurrentWeapon;

	// 투구
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AItemBase> Helmet;

	// 갑옷
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AItemBase> Armor;

	// 신발
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AItemBase> Shoes;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	ECustomMovementMode CurrentMovementMode = ECustomMovementMode::Run;

	// 원래 이동 모드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	ECustomMovementMode OriginalMovementMode = ECustomMovementMode::Crouch;

	// 몽타주 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponType, Category = "C++")
	EItem WeaponType;
};

