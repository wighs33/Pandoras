// 인벤토리 룸
// I를 누르면 캐릭터가 인벤토리 룸으로 이동

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
	// 컴포넌트 생성 직후 호출
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
	// 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	
	// 메인 스프링 암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> MainSpringArm;

	// 무기 스프링 암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> WeaponSpringArm;

	// 머리 스프링 암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> HeadSpringArm;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	// 회전하는 캡슐
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> RotateCapsule;

	// 이 위치로 캐릭터 소환
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> PlayerPosition;

	// 배경 돔 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> DomeMesh;

	// 소환된 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACharacter> PlayerCharacter;

	// 인벤토리 열려있는 지 체크하는 플래그
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bInventoryOpen = false;

	// 인게임에서 메시와 캐릭터의 상대적인 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FTransform MeshRelativeTransformOrigin = FTransform();

	// 인벤토리에 소환된 시점의 메시와 캐릭터의 상대적인 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	FTransform MeshRelativeTransformInventory = FTransform();

	// 메시가 회전 중인지 체크하는 플래그
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bMeshRotating = false;
};
