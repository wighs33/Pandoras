// �÷��̾� ������Ʈ (������ ������ ����)

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
	// ���� ������ ���� �ݵ�� GetLifetimeReplicatedProps �� �������̵�
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> SwordAbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> BowAbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> GloveAbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> HelmetAbilityClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "C++", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGA_Equip>> ArmorAbilityClasses;
};
