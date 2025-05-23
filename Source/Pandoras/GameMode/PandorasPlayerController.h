// �ǵ����� ���� �÷��̾� ��Ʈ�ѷ�

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/PlayerControllerInterface.h"
#include "PandorasPlayerController.generated.h"

UCLASS()
class PANDORAS_API APandorasPlayerController : 
	public APlayerController,
	// ��Ʈ�ѷ� �������̽�
	public IPlayerControllerInterface
{
	GENERATED_BODY()
	
//protected:
//	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FString KeyNames;
};
