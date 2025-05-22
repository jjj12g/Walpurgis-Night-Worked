// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/pixelPlayerState.h"
#include "Player/StateComponent.h"
#include "MyGameModeBase.h"
#include "Player/Widget/NormallyWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "Player/PlayerStatWidget.h"
#include "PCodePlayerController.h"
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>




ApixelPlayerState::ApixelPlayerState()
{
    bReplicates = true;
}

void ApixelPlayerState::SetaddUpEXP(float AcquireEXP)
{
    maxEXP();

    // 현재 경험치 추가
	currentEXP += AcquireEXP;
    auto* GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GM)
    {
    	// GameMode에 접근하여 원하는 작업 수행
    	GM->EXPmanagement(AcquireEXP, this);
    }
    // 경험치 업데이트 후 최대 경험치 체크
    
    // OnRep 함수 호출
    OnRep_currentEXP(currentEXP);
    
}

void ApixelPlayerState::LevelUP()
{
    Level += 1;
    maxEXP(); // 레벨 업 시 최대 경험치 업데이트

   auto* APc = Cast<APCodePlayerController>(GetPawn()->GetController());
   if (APc != nullptr)
   {
       APc->FullExp();
   }
}

void ApixelPlayerState::InitPlayerData()
{
    Level = 1;
    maxEXP(); // 초기화 시 최대 경험치 설정
    currentEXP = 0.0f;
}

float ApixelPlayerState::GetCurrentExp() const
{
    

    return currentEXP;
}

int32 ApixelPlayerState::GetCharacterLevel() const
{
    return Level;
}

void ApixelPlayerState::maxEXP()
{
    if (Level == 1)
    {
        totalEXP = 150;
    }
    else if (Level == 2)
    {
        totalEXP = 400;
    }
    else if (Level == 3)
    {
        totalEXP = 650;
    }
    else if (Level == 4)
    {
        totalEXP = 900;
    }
    else if (Level == 5)
    {
        totalEXP = 1150;
    }
}

void ApixelPlayerState::OnRep_totalEXP(int32 OldEXP)
{
    // totalEXP가 변경될 때 실행할 로직 작성
    //UE_LOG(LogTemp, Warning, TEXT("Total EXP changed from %d to %d"), OldEXP, totalEXP);
}

void ApixelPlayerState::OnRep_currentEXP(float OldEXP)
{
    // currentEXP가 변경될 때 실행할 로직 작성
	//UE_LOG(LogTemp, Warning, TEXT("Current EXP changed from %f to %f"), OldEXP, currentEXP);
}

