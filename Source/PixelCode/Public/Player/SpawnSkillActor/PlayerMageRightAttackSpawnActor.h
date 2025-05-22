// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMageRightAttackSpawnActor.generated.h"

class UParticleSystem;
class USceneComponent;
class USphereComponent;
class ACharacter;
class AActor;
class ABossApernia;
class ADemonSword;
class AGrux;
class ADogBart;
class APlayerObjectPoolManager;


UCLASS()
class PIXELCODE_API APlayerMageRightAttackSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMageRightAttackSpawnActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Components")  // 보통 VisibleAnywhere로 선언하여 에디터에서 보이도록 설정
    USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, Category = "Components");
	USphereComponent * SphereComp1;


	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageRightAttack;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* NA_MageRightAttackComp1;

	 UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<APawn>> EnemyClasses; // 공격할 적 클래스 배열


	float Speed = 1000.f;

	UPROPERTY(EditAnywhere)
    float AttackRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly)
	float MoveSpeed = 20.0f;

	float DestroyTime = 0.0f;

	UPROPERTY(EditAnywhere)
    float DestroyDelay = 2.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount = 10;

	bool bDestroy = false;

	bool bIsTargetFound = false;

	FVector TargetLocation; // 가장 가까운 적의 위치를 저장하는 변수

	UFUNCTION()
	void OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	FVector GetTargetEnemyLocation();
	
};
