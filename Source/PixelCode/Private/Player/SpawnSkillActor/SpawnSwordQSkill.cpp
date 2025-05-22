// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/SpawnSwordQSkill.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"

// Sets default values
ASpawnSwordQSkill::ASpawnSwordQSkill()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

}

// Called when the game starts or when spawned
void ASpawnSwordQSkill::BeginPlay()
{
	Super::BeginPlay();
	
    // 초기 위치 설정
    SetActorLocation(GetActorLocation() + GetActorForwardVector()*300); // 예시로 액터를 Z축으로 100만큼 이동시킴
	bDestroy = true;

	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NS_SkillQ, GetActorLocation(), GetActorRotation());
	
}

// Called every frame
void ASpawnSwordQSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    DestroyTime += DeltaTime;
    bhitTime += DeltaTime;

        if (bhitTime >= 0.2f)
        {
            DealDamageToActorsInTrace();
            bhitTime = 0.0f;
        }
		
		
		if (DestroyTime >= 1.2f)
		{ 
			//UE_LOG(LogTemp, Warning, TEXT("destroy"));
			bDestroy = false;
			DestroyTime = 0.0f;

			Destroy();
		}
}

void ASpawnSwordQSkill::DealDamageToActorsInTrace()
{
    FVector StartLocation = GetActorLocation();
    FVector EndLocation = StartLocation + GetActorForwardVector() * TraceDistance;

    FCollisionQueryParams TraceParams(FName(TEXT("DamageTrace")), true, this);
    TraceParams.bTraceComplex = true;
    TraceParams.bReturnPhysicalMaterial = false;

    TArray<FHitResult> HitResults;
    bool bHit = GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::Identity, ECC_PhysicsBody, FCollisionShape::MakeSphere(TraceRadius), TraceParams);

    if (bHit)
    {
        for (const FHitResult& HitResult : HitResults)
        {
            AActor* HitActor = HitResult.GetActor();
            ABossApernia* boss = Cast<ABossApernia>(HitActor);
            ADemonSword* demonSword = Cast<ADemonSword>(HitActor);
            AGrux* grux = Cast<AGrux>(HitActor);
            ADogBart* dogBart = Cast<ADogBart>(HitActor);
            if (boss)
            {
                // 특정 액터 타입인지 확인
                ABossApernia* EnemyCharacter = boss;
                if (EnemyCharacter)
                {
                    // 피해를 입히는 예시: TakeDamage 함수 호출
                    EnemyCharacter->BossTakeDamage(DamageAmount);

                    // 피해 입힌 액터 로그 출력
                    //UE_LOG(LogTemp, Warning, TEXT("boss: %s"), *HitActor->GetName());
                    //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceRadius, 32, FColor::Green, false, 2.0f);
                }
            }
            else if (demonSword)
            {
                ADemonSword* EnemyCharacter = demonSword;
                if (EnemyCharacter)
                {
                    // 피해를 입히는 예시: TakeDamage 함수 호출
                    EnemyCharacter->SwordTakeDamage(DamageAmount);
                }
            }
            else if (grux)
            {
                AGrux* EnemyCharacter = grux;
                if (EnemyCharacter)
                {
                    // 피해를 입히는 예시: TakeDamage 함수 호출
                    EnemyCharacter->GruxTakeDamage(DamageAmount);
                }
            }
            else if (dogBart)
            {
                ADogBart* EnemyCharacter = dogBart;
                if (EnemyCharacter)
                {
                    // 피해를 입히는 예시: TakeDamage 함수 호출
                    EnemyCharacter->DogBartTakeDamage(DamageAmount);
                }
            }
        }
    }
}






