// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/StateComponent.h"
#include "Player/PlayerOrganism.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "PCodeGameInstance.h"
#include "Player/Widget/NormallyWidget.h"
#include "PCodePlayerController.h"

// Sets default values for this component's properties
UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UStateComponent::BeginPlay()
{
	Super::BeginPlay();

	character = GetOwner<APlayerOrganism>();
	Pc = Cast<APCodePlayerController>(GetWorld()->GetFirstPlayerController());

	if (GetIsReplicated() != true)
	{

		SetIsReplicated(true);
	}

	
}


// Called every frame
void UStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UStateComponent::InitStat()
{
	if (GetOwner() == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("GetOwner Is Null : %s, %d"), __FILE__, __LINE__);
		return;
	}

	auto gi = GetOwner()->GetGameInstance<UPCodeGameInstance>();
	if (gi != nullptr)
	{
		stat = gi->GetCharacterDataTable(character->GetName());

		currentHP = stat.MaxHP;
		currentSP = stat.MaxSP;
		currentMP = stat.MaxMP;
		currentATK = stat.ATK;
		currentDEF = stat.DEF;
		currentCritical = stat.Critical;
		currentSTR = stat.STR;
		currentDex = stat.Dex;
		currentInt = stat.INT;
		currentluck = stat.LUCK;
		currentCon = stat.CON;
		
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("GetGameInstance Is Null"));
	}
}

float UStateComponent::GetStatePoint(EStateType stateType)
{
	switch (stateType)
	{
	case HP:
		return currentHP;
	case SP:
		return currentSP;
	case MP:
		return currentMP;
	default:
		//UE_LOG(LogTemp, Warning, TEXT("Type Error, %s, %d"), __FILE__, __LINE__);
		break;
	}

	return 0.0f;
}

float UStateComponent::AddStatePoint(EStateType stateType, float value)
{
	float temp = 0.0f;

	switch (stateType)
	{
	case HP:
		temp = currentHP + value;
		currentHP = temp;

		if (currentHP <= 0)
		{
			if (dieDelegate.IsBound())
			{
				dieDelegate.Execute();
			}
		}

		break;
	case SP:
		temp = currentSP + value;
		currentSP = temp;

		break;
	case MP:
		temp = currentMP + value;
		currentMP = temp;
		
		break;
	default:
		//UE_LOG(LogTemp, Warning, TEXT("Type Error, %s, %d"), __FILE__, __LINE__);
		break;
	}

	return temp;
}

void UStateComponent::ServerRPC_SetStatePoint_Implementation(EStateType stateType, float value)
{
	switch (stateType)
	{
	case HP:
		currentHP = value;
		break;
	case SP:
		currentSP = value;
		break;
	case MP:
		currentMP = value;
		break;
	default:
		//UE_LOG(LogTemp, Warning, TEXT("Type Error, %s, %d"), __FILE__, __LINE__);
		break;
	}
}

void UStateComponent::NetMulticastRPC_SetStatePoint_Implementation(EStateType stateType, float value)
{
	switch (stateType)
	{
	case HP:
		currentHP = value;
		break;
	case SP:
		currentSP = value;
		break;
	case MP:
		currentMP = value;
		break;
	default:
		//UE_LOG(LogTemp, Warning, TEXT("Type Error, %s, %d"), __FILE__, __LINE__);
		break;
	}
}

void UStateComponent::UpdateStat()
{
	MaxHP = stat.MaxHP;
	MaxSP = stat.MaxSP;
	MaxMP = stat.MaxMP;
}




void UStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStateComponent, currentHP);
	DOREPLIFETIME(UStateComponent, MaxHP);
	DOREPLIFETIME(UStateComponent, currentSP);
	DOREPLIFETIME(UStateComponent, MaxSP);
	DOREPLIFETIME(UStateComponent, currentMP);
	DOREPLIFETIME(UStateComponent, MaxMP);
}

