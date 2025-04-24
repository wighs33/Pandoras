// �κ��丮 ��
// I�� ������ �κ��丮 ������ �̵�

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
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	void EnterInventoryMode();

	UFUNCTION(BlueprintCallable, Category = "C++")
	void ExitInventoryMode();

	UFUNCTION(BlueprintCallable, Category = "C++")
	void ChangeFocusPoint(ECharacterFocusPoint focus_point);

	UFUNCTION(BlueprintCallable, Category = "C++")
	void OnCapsuleClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);

	UFUNCTION(BlueprintCallable, Category = "C++")
	void OnCapsuleReleased(UPrimitiveComponent* ClickedComp, FKey ButtonPressed);
	
	UFUNCTION(BlueprintCallable, Category = "C++")
	void ResetMeshRotation();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> MainSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> WeaponSpringArm;

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

	// ĳ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACharacter> PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bInventoryOpen = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FTransform MeshRelativeTransformOrigin = FTransform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FTransform MeshRelativeTransformInventory = FTransform();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bMeshRotating = false;
};
