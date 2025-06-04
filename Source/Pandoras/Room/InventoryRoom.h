// �κ��丮 ��
// I�� ������ ĳ���Ͱ� �κ��丮 ������ �̵�

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Common/Enums.h"
#include "InventoryRoom.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UCapsuleComponent;

UCLASS()
class PANDORAS_API AInventoryRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventoryRoom();

protected:
	// ������Ʈ ���� ���� ȣ��
	virtual void PostInitializeComponents() override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void EnterInventoryMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ExitInventoryMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ChangeFocusPoint(ECharacterFocusPoint focus_point);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnCapsuleClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnCapsuleReleased(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void ResetMeshRotation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void StopMeshRotation();

private:
	// ��Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	
	// ���� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> MainSpringArm;

	// ���� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> WeaponSpringArm;

	// �Ӹ� ������ ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> HeadSpringArm;

	// ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	// ȸ���ϴ� ĸ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> RotateCapsule;

	// �� ��ġ�� ĳ���� ��ȯ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> PlayerPosition;

	// ��� �� �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DomeMesh;

	// ��ȯ�� ĳ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACharacter> PlayerCharacter;

	// �κ��丮 �����ִ� �� üũ�ϴ� �÷���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bInventoryOpen = false;

	// �ΰ��ӿ��� �޽ÿ� ĳ������ ������� ��ġ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FTransform MeshRelativeTransformOrigin = FTransform();

	// �κ��丮�� ��ȯ�� ������ �޽ÿ� ĳ������ ������� ��ġ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FTransform MeshRelativeTransformInventory = FTransform();

	// �޽ð� ȸ�� ������ üũ�ϴ� �÷���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bMeshRotating = false;
};
