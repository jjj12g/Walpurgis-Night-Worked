// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCodePlayerController.generated.h"


class AMyGameModeBase;
class UNormallyWidget;
class UPlayerStatWidget;
class ApixelPlayerState;
class APixelCodeCharacter;
class UStateComponent;
class APlayerObjectPoolManager;

/**
 * 
 */
UCLASS()
class PIXELCODE_API APCodePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	class APlayerController* PlayerController;


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


	void OpenUI(bool bOpen); 


	void SpawnCharacterAtLocation(APixelCodeCharacter* APlayerchar); // 위치에 캐릭터 스폰

	void DeleteCharacter(APixelCodeCharacter* APlayerchar, const FVector& Location);

	void HandleCharacterDeath(); // 캐릭터 사망 처리

	UFUNCTION(Server, Reliable)
	void Server_SpawnAndPossessCharacter(APixelCodeCharacter* CharacterToSpawn);


	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnCharacterAtLocation();

	UFUNCTION(Client, Reliable)
	void ClientRPC_PlayerSpawnWidget();


	
	UPROPERTY(Replicated)
	APlayerObjectPoolManager* ObjectPoolManager; // 오브젝트 풀 관리자



public:
	UPROPERTY()
	AMyGameModeBase* GM;

	UPROPERTY()
	UPlayerStatWidget* statWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPlayerStatWidget> StatWidgetClass;

	UPROPERTY()
	UNormallyWidget* NormallyWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UNormallyWidget> NormallyWidgetClass;
	
	ApixelPlayerState* pixelPlayerState;

	APixelCodeCharacter* MainPlayer;

	UStateComponent* StatComponent;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// UI

	void FullExp();

	void PlayerLevelUp();

	bool ValidatePlayerState();

	void PlayerWidgetUpdate();

	UFUNCTION(Client, Reliable)
	void ClientRPC_PlayerStartWidget();

	void PlayerBeginWidget();

	void PlayerStatWidget();

	void PlayerStopWidget();

	void PlayerBaseSkillWidget();

	void PlayerSwordSkillWidget();

	void PlayerMageSkillWidget();

	void PlayerDieWidget();

	bool bIsStatVisible = true;

	bool bPlayerState = false;


	class AMyGameModeBase* MyGameMode;

};
