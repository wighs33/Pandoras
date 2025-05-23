// �ǵ����� ���� ĳ������ ���̽�

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
	// ������ ��� �������̽�
	public IItemWielderInterface, 
	// ĳ���� ���� �������̽�
	public ICharacterInterface,
	// ĳ������ �����Ƽ ���� �������̽�
	public ICharacterGameAbilityInterface,
	// �Ĺ� �������̽�
	public IFactionsInterface
{
	GENERATED_BODY()

public:
	// ������
	APandorasCharacterBase();
			
protected:
	// ������Ʈ ���� ���� ȣ��
	virtual void PostInitializeComponents() override;

	// ���� ������ ���� �ݵ�� GetLifetimeReplicatedProps �� �������̵�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ��Ʈ�ѷ��� ������ ����� �� ȣ��
	virtual void NotifyControllerChanged() override;

	// ���� ��Ʈ�ѷ��� ������ �� ȣ��
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	// �������� ����
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// ī�޶� ����
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

// �׼�
protected:
	// �̵�
	void Move(const FInputActionValue& Value);

	// �ü�
	void Look(const FInputActionValue& Value);

	// ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Attack();

	// ���� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void StopAttack();

	// ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void LockOn();

	// ���ŷ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Block();

	// ���ŷ ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void StopBlocking();

	// �Ǵϼ� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void FinishAttack();

	// ȸ��
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Evade();

	// �ȱ� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ToggleWalk();

	// �ɱ׷� �ɱ� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ToggleCrouch();

// OnRep_X: �� ���� �� Ŭ�󿡼� ȣ��
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
	// ���� �ϼ� ���� �ӽ÷� ��縣��Ʈ���� ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_DestroyItem_Server(EItem ItemType);
	// ���� .h�� ���Ǵ� ������ .cpp���� ����
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

// ��Ʈ����Ʈ
protected:
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Actor Attributes")
	void UpdateHealth(const float NewHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Actor Attributes")
	void UpdateStamina(const float NewStamina);

// �̺з�
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
	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	// ���� ���ؽ�Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// ���� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// �̵� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// �ü� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// ���� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;

	// ���� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LockOnAction;

	// ���ŷ �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BlockAction;

	// �Ǵϼ� ���� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FinishAttackAction;

	// ȸ�� �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EvadeAction;

	// �ȱ� ��ȯ �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ToggleWalkAction;

	// �ɱ׷� �ɱ� ��ȯ �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ToggleCrouchAction;

	// �⺻ ��Ʈ����Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<const UBaseActorAttributes> BaseActorAttributes;

	// ���� �����Ƽ �ý��� ������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// ���� �����Ƽ �ý��� ������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UCharacterTrajectoryComponent> CharacterTrajectory;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "C++")
	TObjectPtr<AItemBase> CurrentWeapon;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AItemBase> Helmet;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AItemBase> Armor;

	// �Ź�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AItemBase> Shoes;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Dead, Category = "C++")
	bool bDead;

	// ��Ÿ�� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MontageData, Category = "C++")
	FMontage MontageData;

	// �⺻���� �����Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// �� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++")
	AActor* LockTarget;

	// �⺻���� ������ ���� �����Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGameplayAbility>> DefaultWeaponAbilities;

	// �Ĺ� ���̵�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 FactionId;

	// ���� �� �� �¿� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	EAttackState AttackState;

	// ���� �̵� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	ECustomMovementMode CurrentMovementMode = ECustomMovementMode::Run;

	// ���� �̵� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	ECustomMovementMode OriginalMovementMode = ECustomMovementMode::Crouch;

	// ��Ÿ�� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponType, Category = "C++")
	EItem WeaponType;
};

