// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnSwordQSkill.generated.h"

class UNiagaraSystem;
class USceneComponent;

UCLASS()
class PIXELCODE_API ASpawnSwordQSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnSwordQSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Components")  // 보통 VisibleAnywhere로 선언하여 에디터에서 보이도록 설정
    USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NS_SkillQ;

	float DestroyTime = 0.0f;
	float bhitTime = 0.0f;

	bool bDestroy = false;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DamageAmount = 4;

	 UPROPERTY(EditDefaultsOnly, Category = "Skill")
    float TraceDistance = 300.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Skill")
    float TraceRadius = 300.0f;

	UFUNCTION()
    void DealDamageToActorsInTrace();

};
