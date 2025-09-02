// ������ ��Ʋ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ItemInterface.h"
#include "Common/Enums.h"
#include "ItemBase.generated.h"

class USkeletalMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class PANDORAS_API AItemBase : public AActor, public IItemInterface
{
	GENERATED_BODY()

public:
	AItemBase();


protected:
	virtual void InitLeadPose_Implementation(USkeletalMeshComponent* LeadMesh) override;
	virtual FGameplayTagContainer GetItemTag_Implementation() override { return Tag; }
	virtual EItem GetItemType_Implementation() override { return ItemType; }
	virtual void GetItemData_Implementation(float& DamageRadius, TSubclassOf<UGameplayEffect>& GE_Damage_Charged, TSubclassOf<UGameplayEffect>& GE_Damage_lite, TSubclassOf<UGameplayEffect>& GE_Stun) override;

// ������Ʈ
protected:
	// ��Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// ������ ���̷�Ż �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SK_Item;

// ����
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayTagContainer Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	EItem ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> GE_Damage_Charged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> GE_Damage_lite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> GE_Stun;
};
