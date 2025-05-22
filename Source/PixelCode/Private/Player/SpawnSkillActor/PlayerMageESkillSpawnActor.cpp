// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/PlayerMageESkillSpawnActor.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include "BoundCollision.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h>
#include "PCodePlayerController.h"
#include "Player/PixelCodeCharacter.h"


// Sets default values
APlayerMageESkillSpawnActor::APlayerMageESkillSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

	SphereComp->SetGenerateOverlapEvents(true);

	SphereComp->SetRelativeScale3D(FVector(15,15,15));


}

// Called when the game starts or when spawned
void APlayerMageESkillSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	

	SetActorLocation(GetActorLocation());
	bDestroy = true;
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerMageESkillSpawnActor::OnOverlapEnemy);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerMageEskillSound, GetActorLocation());

}

// Called every frame
void APlayerMageESkillSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDestroy)
	{
		DestroyTime += DeltaTime;
		if (DestroyTime >= 0.1f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("destroy"));
			bDestroy = false;
			DestroyTime = 0.0f;
			Destroy();
		}
	}

}

void APlayerMageESkillSpawnActor::OnOverlapEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABossApernia* boss = Cast<ABossApernia>(OtherActor);
	ADemonSword* demonSword = Cast<ADemonSword>(OtherActor);
	AGrux* grux = Cast<AGrux>(OtherActor);
	ADogBart* dogBart = Cast<ADogBart>(OtherActor);
	APCodePlayerController* Pc = Cast<APCodePlayerController>(GetWorld()->GetFirstPlayerController());
	APixelCodeCharacter* Player = Cast<APixelCodeCharacter>(Pc->GetPawn());

	if (boss)
	{
		// 데미지 적용 예시: TakeDamage 함수 호출
		boss->BossTakeDamage(DamageAmount);
		if (NS_MageESkillhit != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_MageESkillhit, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
		}
	}
	
	else if (demonSword)
	{
		demonSword->SwordTakeDamage(DamageAmount);
		if (NS_MageESkillhit != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_MageESkillhit, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
		}
	}
	else if (grux)
	{
		grux->GruxTakeDamage(DamageAmount);
		grux->ServerRPC_TakeDamage();
		if (NS_MageESkillhit != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_MageESkillhit, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
			//UE_LOG(LogTemp, Warning, TEXT("Gruxdamage"));
		}
	}
	else if (dogBart)
	{
		dogBart->DogBartTakeDamage(DamageAmount);
		//dogBart->ServerRPC_TakeDamage();
		FVector PlayerLoc = Player->GetActorLocation();
		FVector dogBartLocation = dogBart->GetActorLocation();
		FVector direction = dogBartLocation - PlayerLoc;
		direction.Normalize();


		dogBart->LaunchCharacter(direction * launchForce, true, true);

		
		if (NS_MageESkillhit != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_MageESkillhit, OtherActor->GetActorLocation(),OtherActor->GetActorRotation());
		}
		//UE_LOG(LogTemp,Warning,TEXT("dogbartdamage"));
	}

}

