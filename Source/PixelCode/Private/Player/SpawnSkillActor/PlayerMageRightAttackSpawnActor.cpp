// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/PlayerMageRightAttackSpawnActor.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystemComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Engine/OverlapResult.h>
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/Pooling/PlayerObjectPoolManager.h"

// Sets default values
APlayerMageRightAttackSpawnActor::APlayerMageRightAttackSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	SphereComp1 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp1"));
	SphereComp1->SetupAttachment(RootComponent);

	SphereComp1->SetGenerateOverlapEvents(true);

	NA_MageRightAttackComp1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp1"));
	NA_MageRightAttackComp1->SetupAttachment(SphereComp1);


}

// Called when the game starts or when spawned
void APlayerMageRightAttackSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	bDestroy = true;
	NA_MageRightAttackComp1 = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRightAttack, GetActorLocation(), GetActorRotation());


	SphereComp1->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageRightAttackSpawnActor::OnOverlapEnemy);

	
	FVector PlayerLocation = GetActorLocation(); // 플레이어 위치

	// 각 위치에서 스피어컬리전을 발사합니다.
	SphereComp1->SetWorldLocation(PlayerLocation);


	
}

// Called every frame
void APlayerMageRightAttackSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 플레이어 이동 로직
	GetTargetEnemyLocation();

	if (bIsTargetFound)
	{
		// 타겟 위치로 부드럽게 이동
		FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSpeed);
		SetActorLocation(NewLocation);
	}
	else
	{
		// 타겟을 찾지 못했을 때 기본 이동 로직 적용
		FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;
		SetActorLocation(NewLocation);
	}


	if (bDestroy)
	{
		DestroyTime += DeltaTime;
		if (DestroyTime >= DestroyDelay)
		{
			//UE_LOG(LogTemp, Warning, TEXT("destroy"));
			bDestroy = false;
			DestroyTime = 0.0f;
			Destroy();
		}
	}
}

FVector APlayerMageRightAttackSpawnActor::GetTargetEnemyLocation()
{
	TArray<AActor*> FoundActors;
	FVector PlayerLocation = GetActorLocation();
	TArray<AActor*> ActiveEnemies;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		APawn* Pawn = Cast<APawn>(Actor);
		if (Pawn && !Pawn->IsPlayerControlled())
		{
			bool bIsValidTarget = false;
			for (const TSubclassOf<APawn>& EnemyClass : EnemyClasses)
			{
				if (Pawn->IsA(EnemyClass))
				{
					bIsValidTarget = true;
					break;
				}
			}

			if (bIsValidTarget)
			{
				float DistanceToEnemy = FVector::Dist(PlayerLocation, Pawn->GetActorLocation());
				if (DistanceToEnemy <= AttackRange)
				{
					TargetLocation = Pawn->GetActorLocation();
					bIsTargetFound = true;
					return TargetLocation;
				}
			}
		}
	}

	bIsTargetFound = false;
	return PlayerLocation;
}



void APlayerMageRightAttackSpawnActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABossApernia* boss = Cast<ABossApernia>(OtherActor);
	ADemonSword* demonSword = Cast<ADemonSword>(OtherActor);
	AGrux* grux = Cast<AGrux>(OtherActor);
	ADogBart* dogBart = Cast<ADogBart>(OtherActor);
	if (boss)
	{
		// 데미지 적용 예시: TakeDamage 함수 호출
		boss->BossTakeDamage(DamageAmount);
		Destroy();
	}
	else if (demonSword)
	{
		demonSword->SwordTakeDamage(DamageAmount);
		Destroy();
	}
	else if (grux)
	{
		grux->GruxTakeDamage(DamageAmount);
		Destroy();
	}
	else if (dogBart)
	{
		dogBart->DogBartTakeDamage(DamageAmount);
		Destroy();
	}

}









