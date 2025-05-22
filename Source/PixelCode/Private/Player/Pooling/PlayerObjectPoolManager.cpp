// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Pooling/PlayerObjectPoolManager.h"
#include "Player/PixelCodeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
APlayerObjectPoolManager::APlayerObjectPoolManager()
{
	PrimaryActorTick.bCanEverTick = false;
	MaxPoolSize = 0; 
}

// Called when the game starts or when spawned
void APlayerObjectPoolManager::BeginPlay()
{
	Super::BeginPlay(); 
	
    for (int32 i = 0; i < MaxPoolSize; i++)
    {
        APixelCodeCharacter* NewCharacter = GetWorld()->SpawnActor<APixelCodeCharacter>(CharacterClass);
        NewCharacter->SetActorHiddenInGame(true); 
        NewCharacter->SetActorEnableCollision(false); 
        NewCharacter->GetCharacterMovement()->Deactivate(); 
        Pool.Add(NewCharacter); 
    }
}

// Called every frame
void APlayerObjectPoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



APixelCodeCharacter* APlayerObjectPoolManager::GetPooledCharacter()
{
    for (APixelCodeCharacter* Character : Pool)
    {
        if (Character->IsHidden())
        {
            return Character;
        }
    }

    APixelCodeCharacter* NewCharacter = GetWorld()->SpawnActor<APixelCodeCharacter>(CharacterClass);
    Pool.Add(NewCharacter);
    return NewCharacter;
}

void APlayerObjectPoolManager::ReturnPooledCharacter(APixelCodeCharacter* PlayerChar)
{
    PlayerChar->SetActorHiddenInGame(true); 
    PlayerChar->SetActorEnableCollision(false); 
    PlayerChar->GetCharacterMovement()->Deactivate(); 

}


