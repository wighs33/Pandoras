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
};
