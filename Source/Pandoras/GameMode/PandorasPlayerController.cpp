// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PandorasPlayerController.h"

// 플레이 시작
// [게임 전용 입력 모드]로 설정
void APandorasPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
}
