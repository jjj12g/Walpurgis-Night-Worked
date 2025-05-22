// Fill out your copyright notice in the Description page of Project Settings.


#include "PCodePlayerController.h"
#include "MyGameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "Player/pixelPlayerState.h"
#include "Player/PlayerStatWidget.h"
#include "Player/Widget/NormallyWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include "Player/PixelCodeCharacter.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameStateBase.h>
#include "BossLoadingWidget.h"
#include "DamageWidget.h"
#include "LoadingWidget1.h"
#include "BossEnterWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyMapWidget.h"
#include "MyMapLodingWidget.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "Player/StateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Pooling/PlayerObjectPoolManager.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include "Player/PlayerOrganism.h"


void APCodePlayerController::BeginPlay()
{
	Super::BeginPlay();

	//pixelPlayerState = nullptr;

	if (HasAuthority())
	{
		GM = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), allActors);


	MainPlayer = Cast<APixelCodeCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	StatComponent = Cast<UStateComponent>(MainPlayer->stateComp);

	statWidget = Cast<UPlayerStatWidget>(CreateWidget(GetWorld(), StatWidgetClass));

	NormallyWidget = Cast<UNormallyWidget>(CreateWidget(GetWorld(), NormallyWidgetClass));
	
	ClientRPC_PlayerStartWidget();
	


	if (ValidatePlayerState())
	{
		PlayerWidgetUpdate();
	}

		// 플레이어 스테이트가 유효한지 검사하고 유효한 경우에만 Setup_PC 함수를 호출
		 // ObjectPoolManager 초기화
		// 예시: World에서 AObjectPoolManager 찾기
		for (TActorIterator<APlayerObjectPoolManager> It(GetWorld()); It; ++It)
		{
			ObjectPoolManager = *It;
			break; // 첫 번째 찾은 오브젝트 풀 매니저 사용
		}

		if (!ObjectPoolManager)
		{
			// 오브젝트 풀 매니저가 없는 경우 에러 처리
			UE_LOG(LogTemp, Error, TEXT("ObjectPoolManager를 찾을 수 없습니다!"));
		}
}
	


void APCodePlayerController::FullExp()
{
	NormallyWidget->firstStatedate(pixelPlayerState);
}

void APCodePlayerController::PlayerLevelUp()
{
	pixelPlayerState->SetaddUpEXP(30.0f);
}

bool APCodePlayerController::ValidatePlayerState()
{
	// 플레이어 컨트롤러를 설정하기 전에, 플레이어 스테이트가 유효한지 보장.
	FTimerHandle TimerHandle_PlayerStateCheck;

	if (PlayerState->IsValidLowLevel())
	{
		//UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->IsValid"));
		return true;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState->Is Not Valid"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_PlayerStateCheck, [&]() {
			if (ValidatePlayerState())
			{
				//UE_LOG(LogTemp, Warning, TEXT("0 APC_Gameplay::PlayerState 다시 validate!!!!!!!!"));
				PlayerWidgetUpdate();
			}
			}, 0.2f, false);

		return false;
	}
}


void APCodePlayerController::PlayerWidgetUpdate()
{
	pixelPlayerState = Cast<ApixelPlayerState>(PlayerState);

		if (pixelPlayerState && NormallyWidget != nullptr && statWidget != nullptr)
		{
			NormallyWidget->currentExpUpdate(pixelPlayerState);
			NormallyWidget->firstStatedate(pixelPlayerState);
;			statWidget->UpdateLevel(pixelPlayerState);
		}
}

void APCodePlayerController::ClientRPC_PlayerStartWidget_Implementation()
{

	// 시작
	if (StatWidgetClass)
	{
		if (statWidget != nullptr)
		{
			statWidget->AddToViewport(1);
			statWidget->SetVisibility(ESlateVisibility::Collapsed);
			//UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
		}

	}

	if (NormallyWidgetClass)
	{
		if (NormallyWidget != nullptr)
		{
			NormallyWidget->AddToViewport(-1);
			NormallyWidget->SetVisibility(ESlateVisibility::Visible);
			//UE_LOG(LogTemp, Warning, TEXT("NormalAuth"));
		}
	}


}

void APCodePlayerController::PlayerStatWidget()
{
	if (!MainPlayer->bIsJump)
	{
		if (bIsStatVisible)
		{
			statWidget->DisplayStat();
			bIsStatVisible = false;
			//UE_LOG(LogTemp, Warning, TEXT("StatOn"));
		}

		else
		{
			statWidget->HideStat();
			bIsStatVisible = true;
			//UE_LOG(LogTemp, Warning, TEXT("StatOff"));
		}
	}

}

void APCodePlayerController::PlayerStopWidget()
{
	NormallyWidget->SetActiveStopWidgetUI(true);
}

void APCodePlayerController::PlayerBaseSkillWidget()
{
	NormallyWidget->SetBaseSkillWidget(true);
}

void APCodePlayerController::PlayerSwordSkillWidget()
{
	NormallyWidget->SetSwordSkillWidget(true);
}

void APCodePlayerController::PlayerMageSkillWidget()
{
	NormallyWidget->SetMageSkillWidget(true);
}

void APCodePlayerController::PlayerDieWidget()
{
	if (NormallyWidget)
	{
		NormallyWidget->SetActiveGameOverUI(true);
	}
}


void APCodePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MainPlayer == nullptr)
	{
		MainPlayer = Cast<APixelCodeCharacter>(GetPawn());
		if (MainPlayer == nullptr)
		{
			return;
		}
	}

	if (pixelPlayerState != nullptr && NormallyWidget != nullptr)
	{
		NormallyWidget->currentExpUpdate(pixelPlayerState);
		NormallyWidget->currentLevelUpdate(pixelPlayerState);
	}
	
	if (MainPlayer->stateComp != nullptr && NormallyWidget != nullptr && MainPlayer != nullptr)
	{
		NormallyWidget->currentStatUpdate(StatComponent);
	}
	
}

void APCodePlayerController::OpenUI(bool bOpen)
{
	if (bOpen)
	{
		SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}

	SetShowMouseCursor(bOpen);
}

void APCodePlayerController::HandleCharacterDeath()
{
	if (ObjectPoolManager)
	{
		UnPossess(); // 조종 중지

	}
}

void APCodePlayerController::ClientRPC_PlayerSpawnWidget_Implementation()
{
	StatComponent->InitStat();
}

void APCodePlayerController::ServerRPC_SpawnCharacterAtLocation_Implementation()
{
	APixelCodeCharacter* PooledCharacter = ObjectPoolManager->GetPooledCharacter();
	SpawnCharacterAtLocation(PooledCharacter);

}


void APCodePlayerController::SpawnCharacterAtLocation(APixelCodeCharacter* PooledCharacter)
{
	//SpawnCharacterAtLocation(MainPlayer->GetActorLocation());
	if (ObjectPoolManager)
	{
		// 오브젝트 풀에서 사용 가능한 캐릭터 가져오기
		
		if (PooledCharacter)
		{
			PooledCharacter->bPoss = true;
			PooledCharacter->SetActorLocation(PooledCharacter->GetActorLocation()); // 위치 설정
			PooledCharacter->SetActorHiddenInGame(false); // 게임에서 표시
			PooledCharacter->SetActorEnableCollision(true); // 충돌 활성화
			PooledCharacter->GetCharacterMovement()->Activate(); // 움직임 활성화
			StatComponent = PooledCharacter->stateComp;
			
			ClientRPC_PlayerSpawnWidget();

			NormallyWidget = this->NormallyWidget;
			NormallyWidget->bPlayerDie = true;

		
			if (HasAuthority())
			{
				 //서버 권한이 있는 경우에는 바로 호출
				Possess(PooledCharacter);
			}
			else
			{
				 //서버 권한이 없는 경우에는 서버에 요청
				Server_SpawnAndPossessCharacter(PooledCharacter);
			}
		}
	}
}

void APCodePlayerController::DeleteCharacter(APixelCodeCharacter* APlayerchar, const FVector& Location)
{
	// 일정 시간 후 캐릭터 숨기기
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, APlayerchar]()
		{
			APlayerchar->motionState = ECharacterMotionState::Idle;

			APlayerchar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			APlayerchar->GetMesh()->SetCollisionProfileName("CharacterMesh");
			APlayerchar->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			ServerRPC_SpawnCharacterAtLocation();
			ObjectPoolManager->ReturnPooledCharacter(APlayerchar); // 캐릭터 반환
			
			
		});
}


void APCodePlayerController::Server_SpawnAndPossessCharacter_Implementation(APixelCodeCharacter* CharacterToSpawn)
{
	Possess(CharacterToSpawn);
}


void APCodePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// 오브젝트 풀 관리자 객체를 네트워크로 복제합니다.
	DOREPLIFETIME(APCodePlayerController, ObjectPoolManager);

}




