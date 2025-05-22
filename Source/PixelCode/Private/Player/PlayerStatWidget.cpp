// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStatWidget.h"
#include "Player/StateComponent.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Player/PixelCodeCharacter.h"
#include "PCodePlayerController.h"
#include "Player/pixelPlayerState.h"
#include "MyGameModeBase.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameState.h"




void UPlayerStatWidget::DisplayStat()
{
	SetVisibility(ESlateVisibility::Visible);
	//UE_LOG(LogTemp, Warning, TEXT("StatWidgetOn"));
}

void UPlayerStatWidget::HideStat()
{
	SetVisibility(ESlateVisibility::Collapsed);
	//UE_LOG(LogTemp, Warning, TEXT("StatWidgetOff"));
}

void UPlayerStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UPlayerStatWidget::UpdateStat(UStateComponent* PlayerStateComp)
{
	HP = FString::FromInt(PlayerStateComp->MaxHP);
	TB_HP->SetText(FText::FromString(HP));  

	MP = FString::FromInt(PlayerStateComp->MaxMP);
	TB_MP->SetText(FText::FromString(MP));  

	SP = FString::FromInt(PlayerStateComp->MaxSP);
	TB_SP->SetText(FText::FromString(SP));

	ATK = FString::FromInt(PlayerStateComp->currentATK);
	TB_ATK->SetText(FText::FromString(ATK));  

	DEF = FString::FromInt(PlayerStateComp->currentDEF);
	TB_DEF->SetText(FText::FromString(DEF)); 

	CRITICAL = FString::FromInt(PlayerStateComp->currentCritical);
	TB_Critical->SetText(FText::FromString(CRITICAL)); 

	STR = FString::FromInt(PlayerStateComp->currentSTR);
	TB_STR->SetText(FText::FromString(STR));  

	DEX = FString::FromInt(PlayerStateComp->currentDex);
	TB_DEX->SetText(FText::FromString(DEX)); 

	INT = FString::FromInt(PlayerStateComp->currentInt);
	TB_INT->SetText(FText::FromString(INT));  

	LUCK = FString::FromInt(PlayerStateComp->currentluck);
	TB_LUCK->SetText(FText::FromString(LUCK)); 

	CON = FString::FromInt(PlayerStateComp->currentCon);
	TB_CON->SetText(FText::FromString(CON));  
}

void UPlayerStatWidget::UpdateLevel(ApixelPlayerState* Ps)
{
	LEVEL = FString::FromInt(Ps->Level);  // float을 FString으로 변환
	TB_LEVEL->SetText(FText::FromString(LEVEL));  // FString을 FText로 변환하여 UTextBlock에 설정	
	//UE_LOG(LogTemp, Warning, TEXT("UPdateLEvel"));
}





