// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PandorasAIController.h"
#include "Perception/AIPerceptionComponent.h"

APandorasAIController::APandorasAIController()
{
    Sight = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception_Sight"));
    Hearing = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception_Hearing"));
}
