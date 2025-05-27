// 판도라즈 전용 플레이어 컨트롤러

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/PlayerControllerInterface.h"
#include "PandorasPlayerController.generated.h"

UCLASS()
class PANDORAS_API APandorasPlayerController : 
	public APlayerController,
	// 컨트롤러 인터페이스
	public IPlayerControllerInterface
{
	GENERATED_BODY()
};
