// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "StateComponent.generated.h"

UENUM()
enum EStateType
{
	HP,
	SP,
	MP
};

class APlayerOrganism;
class UNormallyWidget;
class APCodePlayerController;

// DataTable(베이스 포인트)
USTRUCT(BlueprintType)
struct FCharacterStat : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxSP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 ATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 DEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Critical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 STR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Dex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 INT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 LUCK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 CON;

};

class UDataTable;

DECLARE_DYNAMIC_DELEGATE(FCharacterHPZero);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PIXELCODE_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UNormallyWidget* NormallyWidget;
	APCodePlayerController* Pc;

private:

	APlayerOrganism* character;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FCharacterStat stat;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float currentHP = 0.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float MaxHP = 0.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float currentSP = 0.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float MaxSP = 0.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float currentMP = 0.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	float MaxMP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 currentATK = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 currentDEF = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 currentCritical = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 currentSTR = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 currentDex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 currentInt = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 currentluck = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 currentCon = 0;


	void InitStat();

	// Get Current Value
	float GetStatePoint(EStateType stateType);
	float AddStatePoint(EStateType stateType, float value);


	UFUNCTION(Server, Reliable)
	void ServerRPC_SetStatePoint(EStateType stateType, float value);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SetStatePoint(EStateType stateType, float value);

	int32 GetATK() { return currentATK; };
	int32 GetDEF() { return currentDEF; };
	int32 GetCritical() { return currentCritical; };
	int32 GetSTR() { return currentSTR; };
	int32 GetDex() { return currentDex; };
	int32 GetInt() { return currentInt; };
	int32 Getluck() { return currentluck; };
	int32 GetCon() { return currentCon; };


	// Set CurrentValue
	void UpdateStat();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, Category = "MySettings")
	FCharacterHPZero dieDelegate;
};
