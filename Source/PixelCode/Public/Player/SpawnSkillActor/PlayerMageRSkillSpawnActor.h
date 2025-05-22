// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMageRSkillSpawnActor.generated.h"

class UParticleSystem;
class USceneComponent;
class USphereComponent;
class UParticleSystemComponent;
class ABossApernia;
class ADemonSword;
class AGrux;
class ADogBart;

UCLASS()
class PIXELCODE_API APlayerMageRSkillSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerMageRSkillSpawnActor();

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
	UParticleSystem* NA_MageRSkillAttack;

	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageRSkillfire;

	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageRSkillfire2;
	

	UPROPERTY(EditAnywhere)
	UParticleSystem* NA_MageRSkillhit;


	ABossApernia* boss;
	ADemonSword* demonSword;
	AGrux* grux;
	ADogBart* dogBart;

	// Timer handles
	FTimerHandle DamageTimerHandle;

	float DestroyTime = 0.0f;

	bool bDestroy = false;

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
	USoundBase* PlayerMageFirstHitSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerMageRskillSound;

};
