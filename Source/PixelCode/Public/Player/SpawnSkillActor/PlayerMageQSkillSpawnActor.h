// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMageQSkillSpawnActor.generated.h"



class UParticleSystem;
class USceneComponent;
class UParticleSystemComponent;
class UBoxComponent;

UCLASS()
class PIXELCODE_API APlayerMageQSkillSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMageQSkillSpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Components")  // 보통 VisibleAnywhere로 선언하여 에디터에서 보이도록 설정
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components");
	UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageQSkill;

	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageQSkillHit;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* NA_MageQSkillComp;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 50;

	UFUNCTION()
	void OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FVector InitialScale;
	float ElapsedTime = 0.0f;
	float MaxScaleFactor;
	float ScaleSpeed;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerMageQskillSound;

};
