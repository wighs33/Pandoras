// 검 아이템

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Sword.generated.h"

class USoundCue;

UCLASS(Blueprintable, BlueprintType)
class PANDORAS_API ASword : public AItemBase
{
	GENERATED_BODY()

protected:
	// 변수 복제를 위해 반드시 GetLifetimeReplicatedProps 를 오버라이드
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_TrailEnabled();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_BloodTriggered();
	
// 변수
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_TrailEnabled, Category = "C++")
	bool TrailEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_BloodTriggered, Category = "C++")
	bool BloodTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool WeaponCharged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> HitActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TObjectPtr<AActor>> AttackedActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FVector ContactLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<USoundCue> SwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<USoundCue> BlockedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<USoundCue> HitSound;
};
