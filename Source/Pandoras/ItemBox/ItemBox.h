// ������ �ڽ�

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"

class UBoxComponent;
class UGA_Equip;
class AItemBase;
class UMaterialBillboardComponent;
class UWidgetComponent;

UCLASS()
class PANDORAS_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

protected:
	// ������Ʈ ���� ���� ȣ��
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnColliderBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnColliderEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

// RPC
protected:
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "C++")
	void Collect_Server(AActor* OwnerActor);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_Collect_Server(AActor* OwnerActor);
	bool Collect_Server_Validate(AActor* OwnerActor);
	void Collect_Server_Implementation(AActor* OwnerActor);

    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "C++")
    void Collect_Multicast(AActor* OwnerActor);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void BP_Collect_Multicast(AActor* OwnerActor);
	void Collect_Multicast_Implementation(AActor* OwnerActor);

// ������Ʈ
protected:
	// ��Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Chest;

	// �÷��̾� Ʈ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> PlayerCollider;

	// �Ѳ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Lid;

	// ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialBillboardComponent> MaterialBillboard;

	// ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> Widget;

// ����
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TSubclassOf<UGA_Equip>> ItemAbilityClasses;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AItemBase> SpawnedActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> InteractingActor;
};
