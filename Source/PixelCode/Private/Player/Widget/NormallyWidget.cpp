// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/NormallyWidget.h"
#include "Components/ProgressBar.h"
#include "Player/StateComponent.h"
#include "Player/pixelPlayerState.h"
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Image.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/MaterialInstance.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/Material.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "PCodePlayerController.h"
#include "PCodeGameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerState.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Controller.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetSwitcher.h>





void UNormallyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Script/Engine.Material'/Game/Player/PlayerWidget/M_RoundProgressbar.M_RoundProgressbar'_C"));

	//UMaterialInstanceDynamic를 생성
	QDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	EDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	RDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	ZDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);

	mageQDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	mageEDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	mageRDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	mageZDynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);

	QDynamicMaterial->SetScalarParameterValue(TEXT("Percent"),-0.1f);
	EDynamicMaterial->SetScalarParameterValue(TEXT("Percent"),-0.1f);
	RDynamicMaterial->SetScalarParameterValue(TEXT("Percent"),-0.1f);
	ZDynamicMaterial->SetScalarParameterValue(TEXT("Percent"),-0.1f);

	mageQDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), -0.1f);
	mageEDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), -0.1f);
	mageRDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), -0.1f);
	mageZDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), -0.1f);

	BP_QSkillbar->SetBrushFromMaterial(QDynamicMaterial);
	BP_ESkillbar->SetBrushFromMaterial(EDynamicMaterial);
	BP_RSkillbar->SetBrushFromMaterial(RDynamicMaterial);
	BP_ZSkillbar->SetBrushFromMaterial(ZDynamicMaterial);

	BP_mageQSkillbar->SetBrushFromMaterial(mageQDynamicMaterial);
	BP_mageESkillbar->SetBrushFromMaterial(mageEDynamicMaterial);
	BP_mageRSkillbar->SetBrushFromMaterial(mageRDynamicMaterial);
	BP_mageZSkillbar->SetBrushFromMaterial(mageZDynamicMaterial);

	// 리스폰
	BTN_Respawn->OnClicked.AddDynamic(this, &UNormallyWidget::OnMyButtonRespawn);
	BTN_Quit->OnClicked.AddDynamic(this, &UNormallyWidget::OnMyButtonQuit);


	Player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	SwitcherUI->SetActiveWidgetIndex(0);
	SkillSwitcherUI->SetActiveWidgetIndex(0);
}

void UNormallyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);


	if (bPlayerDie)
	{
		Player = Cast<APixelCodeCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
		bPlayerDie = false;
	}

	//currentExpUpdate(PlayerState->currentEXP, PlayerState->totalEXP);

	QSetPercent();
	ESetPercent();
	RSetPercent();
	ZSetPercent();

	mageQSetPercent();
	mageESetPercent();
	mageRSetPercent();
	mageZSetPercent();
}


void UNormallyWidget::firstUpdate(UStateComponent* PlayerStateComp)
{
	PB_HP->SetPercent(PlayerStateComp->MaxHP);
	PB_MP->SetPercent(PlayerStateComp->MaxMP);
	PB_SP->SetPercent(PlayerStateComp->MaxSP);
	//UE_LOG(LogTemp, Warning, TEXT("PlayerStateNonull"));
}
void UNormallyWidget::firstStatedate(ApixelPlayerState* Ps)
{
	PB_Exp->SetPercent(Ps->currentEXP);

	LEVEL = FString::FromInt(Ps->Level);  // float을 FString으로 변환
	TB_LEVEL->SetText(FText::FromString(LEVEL));  // FString을 FText로 변환하여 UTextBlock에 설정
}


void UNormallyWidget::currentStatUpdate(UStateComponent* PlayerStateComp)
{
	if (PlayerStateComp != nullptr)
	{ 
		PB_HP->SetPercent(PlayerStateComp->currentHP/PlayerStateComp->MaxHP);
		PB_MP->SetPercent(PlayerStateComp->currentMP/PlayerStateComp->MaxMP);
		PB_SP->SetPercent(PlayerStateComp->currentSP/PlayerStateComp->MaxSP);
	}
}

void UNormallyWidget::currentExpUpdate(ApixelPlayerState* Ps)
{
	PB_Exp->SetPercent(Ps->currentEXP/Ps->totalEXP);
	//UE_LOG(LogTemp, Warning, TEXT("UPdateEXP"));
}

void UNormallyWidget::currentLevelUpdate(ApixelPlayerState* Ps)
{
	LEVEL = FString::FromInt(Ps->Level);
	TB_LEVEL->SetText(FText::FromString(LEVEL));  
}

void UNormallyWidget::QSetPercent()
{
	if (QDynamicMaterial)
	{
		if (Player!= nullptr && Player->CurrentQSkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			QDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentQSkillCoolTime/Player->QSkillCoolTime);
			//UE_LOG(LogTemp, Warning, TEXT("QskillcollWidget"));
		}

		// BP_QSkillbar가 UImage인 경우 SetBrushFromMaterial을 사용할 수 있습니다.
		BP_QSkillbar->SetBrushFromMaterial(QDynamicMaterial);
		
	}
}

void UNormallyWidget::ESetPercent()
{
	if (EDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentESkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			float PercentValue = 1.0f - Player->CurrentESkillCoolTime / Player->ESkillCoolTime;
			EDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentESkillCoolTime /Player->ESkillCoolTime);
			//UE_LOG(LogTemp, Log, TEXT("Percent set to: %f"), PercentValue);
		}

		BP_ESkillbar->SetBrushFromMaterial(EDynamicMaterial);
	}
}

void UNormallyWidget::RSetPercent()
{
	if (RDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentRSkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			RDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentRSkillCoolTime / Player->RSkillCoolTime);
		}

	
		BP_RSkillbar->SetBrushFromMaterial(RDynamicMaterial);
	}
}

void UNormallyWidget::ZSetPercent()
{
	if (ZDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentZSkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			ZDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentZSkillCoolTime / Player->ZSkillCoolTime);
		}

		
		BP_ZSkillbar->SetBrushFromMaterial(ZDynamicMaterial);
	}
}

void UNormallyWidget::mageQSetPercent()
{
	if (mageQDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentQSkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			mageQDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentQSkillCoolTime / Player->QSkillCoolTime);
		}

		// BP_QSkillbar가 UImage인 경우 SetBrushFromMaterial을 사용할 수 있습니다.
		BP_mageQSkillbar->SetBrushFromMaterial(mageQDynamicMaterial);
	}

}

void UNormallyWidget::mageESetPercent()
{
	if (mageEDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentESkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			float PercentValue = 1.0f - Player->CurrentESkillCoolTime / Player->ESkillCoolTime;
			mageEDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentESkillCoolTime / Player->ESkillCoolTime);
			//UE_LOG(LogTemp, Log, TEXT("Percent set to: %f"), PercentValue);
		}

		BP_mageESkillbar->SetBrushFromMaterial(mageEDynamicMaterial);
	}
}

void UNormallyWidget::mageRSetPercent()
{
	if (mageRDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentRSkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			mageRDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentRSkillCoolTime / Player->RSkillCoolTime);
		}


		BP_mageRSkillbar->SetBrushFromMaterial(mageRDynamicMaterial);
	}
}

void UNormallyWidget::mageZSetPercent()
{
	if (mageZDynamicMaterial)
	{
		if (Player != nullptr && Player->CurrentZSkillCoolTime != 0)
		{
			// 스칼라 파라미터 설정
			mageZDynamicMaterial->SetScalarParameterValue(TEXT("Percent"), 1.0f - Player->CurrentZSkillCoolTime / Player->ZSkillCoolTime);
		}


		BP_mageZSkillbar->SetBrushFromMaterial(mageZDynamicMaterial);
	}
}

void UNormallyWidget::OnMyButtonRespawn()
{
	// 게임오버UI를 보이지않게하고
	SetActiveGameOverUI(false);
	auto* pc = Cast<APCodePlayerController>(GetWorld()->GetFirstPlayerController());

	if (pc)
	{
		// 플레이어컨트롤러를 통해 재시작하고싶다.
		pc->SetInputMode(FInputModeGameOnly());
		pc->SetShowMouseCursor(false);
		pc->DeleteCharacter(Player, Player->GetActorLocation());
	}
}

void UNormallyWidget::OnMyButtonQuit()
{
	auto* gi = Cast<UPCodeGameInstance>(GetWorld()->GetGameInstance());
	if (gi)
	{
		gi->ExitRoom();
	}
}

void UNormallyWidget::SetActiveGameOverUI(bool value)
{
	SwitcherUI->SetActiveWidgetIndex(0);
	SwitcherUI->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CP_GameOverUI->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	//UE_LOG(LogTemp,Warning,TEXT("RespawnUI"));
}

void UNormallyWidget::SetActiveStopWidgetUI(bool value)
{
	SwitcherUI->SetActiveWidgetIndex(1);
	SwitcherUI->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CP_StopWidget->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UNormallyWidget::SetBaseSkillWidget(bool value)
{
	SkillSwitcherUI->SetActiveWidgetIndex(0);
	SkillSwitcherUI->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CP_PlayerBaseState->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UNormallyWidget::SetSwordSkillWidget(bool value)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetSwordSkillWidget"));
	SkillSwitcherUI->SetActiveWidgetIndex(1);
	SkillSwitcherUI->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CP_PlayerSwordSkill->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	//UE_LOG(LogTemp, Warning, TEXT("Boolean value is: %s"), value ? TEXT("True") : TEXT("False"));
}

void UNormallyWidget::SetMageSkillWidget(bool value)
{
	SkillSwitcherUI->SetActiveWidgetIndex(2);
	SkillSwitcherUI->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CP_PlayerMageSkill->SetVisibility(value ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}



