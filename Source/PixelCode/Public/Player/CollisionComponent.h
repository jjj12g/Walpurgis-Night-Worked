// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include <Delegates/DelegateCombinations.h>
#include <Engine/HitResult.h>
#include "CollisionComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnHitDeligate, FHitResult);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PIXELCODE_API UCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCollisionComponent();
	
protected:


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UPrimitiveComponent* collisionMeshComponent;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	FName startSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	FName endSocketName;

	FVector startSocketLocation;
	FVector endSocketLocation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	float traceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	TArray<TEnumAsByte<EObjectTypeQuery>> collisionObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	TEnumAsByte<EDrawDebugTrace::Type> drawDebugType;

	bool bCollisionEnabled = false;
	TArray<AActor*>		alreadyHitActors;
	TArray<AActor*>		actorsToIgnore;
	FHitResult			lastHitStruct;
	AActor*				hittedActor;

	FOnHitDeligate onHitDeligate;

	// Functions
	TArray<AActor*>			GetAlreadyHitActors() { return alreadyHitActors; };
	bool					CheckCollisionEnabled() { return bCollisionEnabled; };
	FHitResult				GetLastHit() { return lastHitStruct; };
	UPrimitiveComponent*	GetCollisionMeshComponent() { return collisionMeshComponent; };
	void					AddActorToIgnore(AActor* addActor);
	void					RemoveActorToIgnore(AActor* removeActor);
	TArray<AActor*>			GetActorsToIgnore() { return actorsToIgnore; };

	// FromCustomEvent
	void SetCollisionMesh(UPrimitiveComponent* meshComp);
	void SetEnableCollision(bool bEnable);
	void ClearHitActor();
	void CollisionTrace();
};
