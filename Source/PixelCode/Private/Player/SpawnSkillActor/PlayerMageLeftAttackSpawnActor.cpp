// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/PlayerMageLeftAttackSpawnActor.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
APlayerMageLeftAttackSpawnActor::APlayerMageLeftAttackSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

	SphereComp->SetGenerateOverlapEvents(true);

	SphereComp->SetRelativeScale3D(FVector(2));

	NA_MageLeftAttackComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	NA_MageLeftAttackComp->SetupAttachment(SphereComp);


	NA_MageLeftAttackComp->SetRelativeScale3D(FVector(2)); // 기본 스케일 1.0
}

// Called when the game starts or when spawned
void APlayerMageLeftAttackSpawnActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(GetActorLocation() + GetActorForwardVector()* 100);
	bDestroy = true;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageLeftAttackSpawnActor::OnOverlapEnemy);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageLeftAttack, GetActorLocation(), GetActorRotation());
	
}

// Called every frame
void APlayerMageLeftAttackSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime; // Speed는 앞으로 이동할 속도
	SetActorLocation(NewLocation);

	//UE_LOG(LogTemp,Warning,TEXT("PlayerLocation"));

	if (bDestroy)
	{
		DestroyTime += DeltaTime;
		if (DestroyTime >= 1.0f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("destroy"));
			bDestroy = false;
			DestroyTime = 0.0f;
			Destroy();
		}
	}

}

void APlayerMageLeftAttackSpawnActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABossApernia* boss = Cast<ABossApernia>(OtherActor);
	ADemonSword* demonSword = Cast<ADemonSword>(OtherActor);
	AGrux* grux = Cast<AGrux>(OtherActor);
	ADogBart* dogBart = Cast<ADogBart>(OtherActor);
	if (boss)
	{
		// 데미지 적용 예시: TakeDamage 함수 호출
		boss->BossTakeDamage(DamageAmount);
	}
	else if (demonSword)
	{
		demonSword->SwordTakeDamage(DamageAmount);
	}
	else if (grux)
	{
		grux->GruxTakeDamage(DamageAmount);
	}
	else if (dogBart)
	{
		dogBart->DogBartTakeDamage(DamageAmount);
	}

	

	UParticleSystemComponent* SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageLefthit, GetActorLocation(), GetActorRotation());
	SpawnedParticleSystem -> SetRelativeScale3D(FVector(3));
	bDestroy = false;
	DestroyTime = 0.0f;
	Destroy();
}

