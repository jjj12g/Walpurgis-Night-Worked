// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortalRobbyWidget.h"
#include "PCodePlayerController.h"
#include "MyGameModeBase.generated.h"


class ApixelPlayerState;

/**
 * 
 */
UCLASS()
class PIXELCODE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameModeBase();


	float totalEXP = 0;
	float currentEXP = 0;

	virtual void BeginPlay() override;

	void EXPmanagement(float EXP, ApixelPlayerState* PlayerState);

	class APCodePlayerController* pc;
	
};
