// 검 아이템

// 모든 부분을 C++로 바꾸려고 했지만 나이아가라 컴포넌트를 C++에서 추가하면 애셋을 넣은 후에
// 자동으로 user parameters가 생기지 않기 때문에 나이아가라 컴포넌트에 접근하는 로직은 블루프린트에서 처리
// 나이아가라 애셋 경로를 넣어주는 방법이 있지만 경로를 하드코딩하는 것은 좋은 방법이 아니기 때문에 보류

#pragma once

#include "Pandoras.h"
#include "Item/ItemBase.h"
#include "Interface/ItemInterface.h"
#include "Sword.generated.h"

class USoundCue;
class UNiagaraComponent;
class UNiagaraSystem;
class UBoxComponent;

UCLASS(Blueprintable, BlueprintType)
class PANDORAS_API ASword : public AItemBase
{
	GENERATED_BODY()
	
public:
	ASword();

protected:
	virtual void BeginPlay() override;

	// 컴포넌트 생성 직후 호출
	virtual void PostInitializeComponents() override;

	// 변수 복제를 위해 반드시 GetLifetimeReplicatedProps 를 오버라이드
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnHitBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnBlockBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_TrailEnabled();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "C++")
	void OnRep_BloodTriggered();

	// 데미지 처리 부분 C++로 구현
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void ApplyDamageAndStun();

// ItemInterface
protected:
	virtual void ToggleHitBox_Implementation(bool bEnable) override;
	virtual void ToggleBlockBox_Implementation(bool bEnable) override;
	virtual void Drop_Implementation() override;

// 컴포넌트(나이아가라 컴포넌트들은 유저 파라미터때문에 블루프린트에서 설정)
protected:
	// 공격 트리거
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> HitBox;

	// 블로킹 트리거
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BlockBox;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayTag DamageTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UDataTable> DT_Item;
};
