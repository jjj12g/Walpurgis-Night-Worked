// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Player/pixelPlayerState.h"
#include "Player/PixelCodeCharacter.h" 
#include "PortalRobbyWidget.h"
#include "PCodePlayerController.h" 
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "PortalCollision.h"
#include "PCodePlayerController.h"
#include "EngineUtils.h" 
#include "Kismet/GameplayStatics.h"

AMyGameModeBase::AMyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	DefaultPawnClass = APixelCodeCharacter::StaticClass();
	PlayerControllerClass = APCodePlayerController::StaticClass();
	PlayerStateClass = ApixelPlayerState::StaticClass();
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
		
}

void AMyGameModeBase::EXPmanagement(float EXP, ApixelPlayerState* PlayerState)
{
	currentEXP += EXP;

	if (PlayerState->totalEXP <= currentEXP)
	{
		PlayerState->LevelUP();
		PlayerState->currentEXP = 0;
	}

}


