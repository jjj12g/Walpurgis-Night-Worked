// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMageLeftAttackSpawnActor.generated.h"


class UParticleSystem;
class USceneComponent;
class USphereComponent;
class UParticleSystemComponent;

UCLASS()
class PIXELCODE_API APlayerMageLeftAttackSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMageLeftAttackSpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Components")  // 보통 VisibleAnywhere로 선언하여 에디터에서 보이도록 설정
    USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components");
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageLeftAttack;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageLefthit;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* NA_MageLeftAttackComp;

	float Speed = 2000.f;

	float DestroyTime = 0.0f;

	bool bDestroy = false;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount = 50;

	UFUNCTION()
	void OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
