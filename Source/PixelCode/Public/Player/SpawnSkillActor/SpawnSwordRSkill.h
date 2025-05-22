// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnSwordRSkill.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class USceneComponent;
class ABossApernia;

UCLASS()
class PIXELCODE_API ASpawnSwordRSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnSwordRSkill();

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

	float Speed = 3100.f;

	float DestroyTime = 0.0f;

	bool bDestroy = false;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount = 50;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NS_SkillR;

	UFUNCTION()
	void OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
