// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/PlayerMageRSkillSpawnActor.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include "BoundCollision.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include "PCodePlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Particles/ParticleSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Player/PixelCodeCharacter.h"

// Sets default values
APlayerMageRSkillSpawnActor::APlayerMageRSkillSpawnActor()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;


	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

	SphereComp->SetGenerateOverlapEvents(true);

	SphereComp->SetRelativeScale3D(FVector(20));

}

// Called when the game starts or when spawned
void APlayerMageRSkillSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(GetActorLocation());
	bDestroy = true;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageRSkillSpawnActor::OnOverlapEnemy);
	
	//UE_LOG(LogTemp, Warning, TEXT("RSKillActor"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRSkillAttack, GetActorLocation(), GetActorRotation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRSkillfire, GetActorLocation(), GetActorRotation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRSkillfire2, GetActorLocation(), GetActorRotation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerMageFirstHitSound, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerMageRskillSound, GetActorLocation());
}

// Called every frame
void APlayerMageRSkillSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDestroy)
	{
		DestroyTime += DeltaTime;
		if (DestroyTime >= 3.0f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("destroy"));
			bDestroy = false;
			DestroyTime = 0.0f;
			GetWorldTimerManager().ClearTimer(DamageTimerHandle);
			Destroy();
		}
	}

}

//
void APlayerMageRSkillSpawnActor::ApplyDamage()
{
	for (int32 i = 0; i < OverlappingActors.Num(); ++i)
	{
		AActor* CurrentActor = OverlappingActors[i];
		bool IsOverlapping = IsOverlappingFlags[i];

		if (IsOverlapping)
		{
			if (ABossApernia* Boss = Cast<ABossApernia>(CurrentActor))
			{
				Boss->BossTakeDamage(DamageAmount);
				if (NA_MageRSkillhit != nullptr)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRSkillhit, Boss->GetActorLocation(), Boss->GetActorRotation());
				}
			}
			else if (ADemonSword* DemonSword = Cast<ADemonSword>(CurrentActor))
			{
				DemonSword->SwordTakeDamage(DamageAmount);
				if (NA_MageRSkillhit != nullptr)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRSkillhit, DemonSword->GetActorLocation(), DemonSword->GetActorRotation());
				}
			}
			else if (AGrux* Grux = Cast<AGrux>(CurrentActor))
			{
				Grux->GruxTakeDamage(DamageAmount);
				Grux->ServerRPC_TakeDamage();
				if (NA_MageRSkillhit != nullptr)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRSkillhit, Grux->GetActorLocation(), Grux->GetActorRotation());
				}
			}
			else if (ADogBart* DogBart = Cast<ADogBart>(CurrentActor))
			{
				DogBart->DogBartTakeDamage(DamageAmount);
				DogBart->ServerRPC_TakeDamage();
				if (NA_MageRSkillhit != nullptr)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), NA_MageRSkillhit, DogBart->GetActorLocation(), DogBart->GetActorRotation());
				}
			}
		}
	}
}





void APlayerMageRSkillSpawnActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	boss = Cast<ABossApernia>(OtherActor);
	demonSword = Cast<ADemonSword>(OtherActor);
	grux = Cast<AGrux>(OtherActor);
	dogBart = Cast<ADogBart>(OtherActor);

	//.Add 메서드를 사용하면 동적으로 배열의 크기 조절, 초기에 배열의 크기를 정하지 않고 필요할 때마다 요소를 추가 가능.
	 // 적이 감지되면 해당 적을 배열에 추가하고 데미지 적용 타이머를 시작합니다.
	if (boss)
	{
		OverlappingActors.Add(OtherActor);
		IsOverlappingFlags.Add(true);
	}
	else if (demonSword)
	{
		OverlappingActors.Add(OtherActor);
		IsOverlappingFlags.Add(true);
	}
	else if (grux)
	{
		OverlappingActors.Add(OtherActor);
		IsOverlappingFlags.Add(true);
	}
	else if (dogBart)
	{
		OverlappingActors.Add(OtherActor);
		IsOverlappingFlags.Add(true);
	}

	// 첫 번째 적이 감지될 때만 데미지를 주기 위해 타이머를 시작합니다.
	if (OverlappingActors.Num() == 1)
	{
		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &APlayerMageRSkillSpawnActor::ApplyDamage, 0.8f, true);
	}
}

void APlayerMageRSkillSpawnActor::EndOverlapEnemy(AActor* OtherActor)
{
	int32 IndexToRemove = OverlappingActors.IndexOfByKey(OtherActor);
	if (IndexToRemove != INDEX_NONE)
	{
		OverlappingActors.RemoveAt(IndexToRemove);
		IsOverlappingFlags.RemoveAt(IndexToRemove);

		if (OverlappingActors.Num() == 0)
		{
			GetWorldTimerManager().ClearTimer(DamageTimerHandle);
		}
	}
}
