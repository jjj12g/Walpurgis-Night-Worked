// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpawnSkillActor/PlayerMageRSkillCastActor.h"
#include "Boss/BossApernia.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include "BoundCollision.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include "PCodePlayerController.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Player/SpawnSkillActor/PlayerMageRSkillSpawnActor.h"
#include "Components/SphereComponent.h"

// Sets default values
APlayerMageRSkillCastActor::APlayerMageRSkillCastActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    // Sphere 컬리전을 가진 컴포넌트 생성
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(100.0f); // 구의 반지름 설정 (적절한 크기로 조정 필요)

    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionSphere->SetSimulatePhysics(true);
    RootComponent = CollisionSphere;

}

// Called when the game starts or when spawned
void APlayerMageRSkillCastActor::BeginPlay()
{
	Super::BeginPlay();
	
    SetActorLocation(GetActorLocation() + FVector(0, 0, 1000));
    SetActorRotation(GetActorRotation());

    FVector LaunchDirection = GetActorRotation().Vector();
    CollisionSphere->AddImpulse(LaunchDirection * 2000);

    // OnActorHit 이벤트에 핸들러 함수를 바인딩
    OnActorHit.AddDynamic(this, &APlayerMageRSkillCastActor::OnMeteorHit);
}

// Called every frame
void APlayerMageRSkillCastActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerMageRSkillCastActor::OnMeteorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor != this)
    {
        boss = Cast<ABossApernia>(OtherActor);
        demonSword = Cast<ADemonSword>(OtherActor);
        grux = Cast<AGrux>(OtherActor);
        dogBart = Cast<ADogBart>(OtherActor);

        //.Add 메서드를 사용하면 동적으로 배열의 크기 조절, 초기에 배열의 크기를 정하지 않고 필요할 때마다 요소를 추가 가능.
        // 적이 감지되면 해당 적을 배열에 추가하고 데미지 적용 타이머를 시작.
       
        // 충돌 지점에 스폰할 액터를 생성
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        APlayerMageRSkillSpawnActor* SpawnedActor = GetWorld()->SpawnActor<APlayerMageRSkillSpawnActor>(ActorToSpawn, Hit.ImpactPoint, FRotator::ZeroRotator, SpawnParams);

        if (SpawnedActor)
        {
            Destroy();
        }
    }
}




