// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMageESkillSpawnActor.generated.h"

class UNiagaraSystem;
class USceneComponent;
class USphereComponent;
class UNiagaraComponent;


UCLASS()
class PIXELCODE_API APlayerMageESkillSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMageESkillSpawnActor();

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
	UNiagaraSystem* NS_MageESkillhit;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	float launchForce = 30000.0f;

	float DestroyTime = 0.0f;

	bool bDestroy = false;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 300;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class TSubclassOf<class ABoundCollision> bounceCollision;

	UFUNCTION()
	void OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerMageEskillSound;

};
