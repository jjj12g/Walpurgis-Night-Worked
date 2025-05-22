// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMageZSkillSpawnActor.generated.h"


class UParticleSystem;
class USceneComponent;
class USphereComponent;
class UParticleSystemComponent;
class ABossApernia;
class ADemonSword;
class AGrux;
class ADogBart;



UCLASS()
class PIXELCODE_API APlayerMageZSkillSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMageZSkillSpawnActor();

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
	UParticleSystem* NA_MageZSkillAttack;

	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MagicCircle;

	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageZSkillhit;



	ABossApernia* boss;
	ADemonSword* demonSword;
	AGrux* grux;
	ADogBart* dogBart;

	// Timer handles
	FTimerHandle DamageTimerHandle;

	float DestroyTime = 0.0f;

	bool bDestroy = false;

	bool bMagicCircle = false;

	float MagicCircleAttackSpawnTime = 0.0f;

	// 데미지 관련 변수
	AActor* OverlappingActor = nullptr;
	bool bIsOverlapping = false;

	// 데미지 처리 관련 변수
	TArray<AActor*> OverlappingActors;
	TArray<bool> IsOverlappingFlags;

	

	void ApplyDamage();

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 10;

	UFUNCTION()
	void OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlapEnemy(AActor* OtherActor);


	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerMageZskillSound;
};
