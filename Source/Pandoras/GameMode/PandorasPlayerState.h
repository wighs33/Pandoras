// 플레이어 스테이트 (아이템 데이터 저장)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/PlayerStateInterface.h"
#include "PandorasPlayerState.generated.h"

class UGA_Equip;

UCLASS()
class PANDORAS_API APandorasPlayerState : public APlayerState, public IPlayerStateInterface
{
	GENERATED_BODY()
	
protected:
	// 변수 복제를 위해 반드시 GetLifetimeReplicatedProps 를 오버라이드
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// IPlayerStateInterface
protected:
	virtual TArray<TSubclassOf<UGA_Equip>> GetItemAbilityClassesByType_Implementation(EItem ItemType) override;
	virtual void AddItemClass_Implementation(TSubclassOf<UGA_Equip> itemAbility) override;
	virtual TArray<TSubclassOf<UGA_Equip>> GetAllItemAbilities_Implementation() override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> SwordAbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> BowAbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> HelmetAbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> ArmorAbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> GlovesAbilityClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> ShoesAbilityClasses;
};
