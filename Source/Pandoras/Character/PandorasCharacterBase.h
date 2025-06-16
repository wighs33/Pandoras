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
	// ������ ��� �������̽�
	public IItemWielderInterface, 
	// ĳ���� ���� �������̽�
	public ICharacterInterface,
	// ĳ������ �����Ƽ ���� �������̽�
	public ICharacterGameAbilityInterface,
	// �Ĺ� �������̽�
	public IFactionsInterface,
	// �����Ƽ �ý��� �������̽�
	public IGASInterface
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

	// ���� ���� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void Sprint();

	// ���� ���� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void QuickSave();

// OnRep_X: �� ���� �� Ŭ�󿡼� ȣ��
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

// ��Ʈ����Ʈ
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

	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "C++")
	float CalculateMovementSpeed();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ClearItemAbilities(AActor* Item);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void SaveCharacterData();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void LoadCharacterData();

// ������Ʈ
protected:
	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// ���� �����Ƽ �ý��� ������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UCharacterTrajectoryComponent> CharacterTrajectory;

// �Է� �׼�
protected:
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

	// ���� ���� ��ȯ �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;

	// ���� ���� ��ȯ �׼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* QuickSaveAction;

// �� �� ����
protected:
	// �⺻ ��Ʈ����Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	TObjectPtr<const UBaseActorAttributes> BaseActorAttributes;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "C++")
	TObjectPtr<AItemBase> CurrentWeapon;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = "C++")
	TObjectPtr<AItemBase> CurrentHelmet;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Armor, Category = "C++")
	TObjectPtr<AItemBase> CurrentArmor;

	// �尩
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Glove, Category = "C++")
	TObjectPtr<AItemBase> CurrentGlove;

	// �Ź�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Shoes, Category = "C++")
	TObjectPtr<AItemBase> CurrentShoes;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentMovementMode, Category = "C++")
	ECustomMovementMode CurrentMovementMode = ECustomMovementMode::Run;

	// ���� �̵� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	ECustomMovementMode OriginalMovementMode = ECustomMovementMode::Crouch;

	// ���� Ÿ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_WeaponType, Category = "C++")
	EItem WeaponType;

	// ���� ���� ���ϸ�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	FString SaveSlot = TEXT("SV_Character");

// ��������Ʈ
protected:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnMaxHealthUpdated OnMaxHealthUpdated;
};

