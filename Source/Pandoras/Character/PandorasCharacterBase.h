// �ǵ����� ���� ĳ������ ���̽�

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Interface/ItemWielderInterface.h"
#include "Interface/CharacterInterface.h"
#include "Common/Structs.h"
#include "Common/Enums.h"

#include "PandorasCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UInputComponent;
class UBaseActorAttributes;
class UAbilitySystemComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// ĳ����, ������ ��� �������̽�, ĳ���� ���� �������̽� ���
UCLASS(config=Game)
class APandorasCharacterBase : public ACharacter, public IItemWielderInterface, public ICharacterInterface
{
	GENERATED_BODY()

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

public:
	// ������
	APandorasCharacterBase();
	
protected:
	// �̵�
	void Move(const FInputActionValue& Value);

	// �ü�
	void Look(const FInputActionValue& Value);
			
protected:
	// ��Ʈ�ѷ��� ������ ����� �� ȣ��
	virtual void NotifyControllerChanged() override;

	// ���� ��Ʈ�ѷ��� ������ �� ȣ��
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	// �������� ����
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// ī�޶� ����
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

//---------------------------------------------------------------------------------------------------------

protected:
	// ������Ʈ ���� ���� ȣ��
	virtual void PostInitializeComponents() override;

	// ���� ������ ���� �ݵ�� GetLifetimeReplicatedProps �� �������̵�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// OnRep_X: �� ���� �� Ŭ�󿡼� ȣ��
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_Dead();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_MontageData();

// ���ø����̼�
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void PlayMontageReplicated(UAnimMontage* AnimMontage, float InPlayRate = 1.0, FName StartSectionName = TEXT("None"));

// RPC
protected:
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void DestroyItem_Server(EItem ItemType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_DestroyItem_Server(EItem ItemType);

    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "C++")
    void DestroyItem_Multicast(EItem ItemType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_DestroyItem_Multicast(EItem ItemType);

	bool DestroyItem_Server_Validate(EItem ItemType);
	void DestroyItem_Server_Implementation(EItem ItemType);
	void DestroyItem_Multicast_Implementation(EItem ItemType);

protected:
	// �⺻ ��Ʈ����Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<const UBaseActorAttributes> BaseActorAttributes;

	// ���� �����Ƽ �ý��� ������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> Weapon;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> Helmet;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> Armor;

	// �Ź�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> Shoes;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Dead, Category = "C++")
	bool bDead;

	// ��Ÿ�� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MontageData, Category = "C++")
	FMontage MontageData;
};

