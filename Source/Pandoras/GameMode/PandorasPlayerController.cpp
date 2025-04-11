// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PandorasPlayerController.h"

void APandorasPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
}
