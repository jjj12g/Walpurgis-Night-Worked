// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseWeapon.h"
#include <GameFramework/Character.h>
#include <Kismet/GameplayStatics.h>
#include "Player/CollisionComponent.h"
#include "Player/CombatComponent.h"
#include "Player/CombatInterface.h"
#include "Boss/BossApernia.h"
#include "Player/AnimInstance_Interface.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "DemonSword.h"
#include "Grux.h"
#include "DogBart.h"
#include "Player/PlayerOrganism.h"
#include "DragonRazorStatue.h"
#include "../../../FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

ABaseWeapon::ABaseWeapon()
{
	collisionComponent = CreateDefaultSubobject<UCollisionComponent>(TEXT("CollisionComponent"));
	ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (collisionComponent != nullptr)
	{
		collisionComponent->onHitDeligate.BindUObject(this, &ABaseWeapon::OnHitCollisionComponent);
	}
	else
	{
		int iTemp = 0;
	}

	
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StartSwordMark = collisionComponent->startSocketLocation;
	EndSwordMark = collisionComponent->endSocketLocation;

}

void ABaseWeapon::OnEquipped()
{
	//Super::OnEquipped();
	if (GetOwner() != nullptr)
	{
		combatComponent = GetOwner()->GetComponentByClass<UCombatComponent>();
		if (combatComponent != nullptr)
		{
			FName selectedName = (combatComponent->bCombatEnable ? handSocketName : AttachSocketName);

			AttachActor(selectedName);

			combatComponent->SetMainWeapon(this);

			ACharacter* characterPtr = Cast<ACharacter>(GetOwner());
			if (characterPtr)
			{
				USkeletalMeshComponent* skeletalMeshComponent = characterPtr->GetMesh();
				if (skeletalMeshComponent)
				{
					UAnimInstance* animInstance = skeletalMeshComponent->GetAnimInstance();

					if (animInstance != nullptr)
					{
						IAnimInstance_Interface* AnimInterface = Cast<IAnimInstance_Interface>(animInstance);

						if (AnimInterface != nullptr)
						{
							AnimInterface->UpdateCombatType_Implementation(eWeaponType);

							//UE_LOG(LogTemp, Warning, TEXT("%s Send UUpdateCombatType"), *animInstance->GetName());
						}
						else
						{
							//UE_LOG(LogTemp, Warning, TEXT("%s Is do not Have IAnimInstance_Interface"), *skeletalMeshComponent->GetName());
						}
					}
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("skeletalMeshComponent Is Nullptr"));
				}
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Owner Is Not Character"));
			}

			collisionComponent->SetCollisionMesh(GetItemMesh());
			collisionComponent->AddActorToIgnore(GetOwner());
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("selectedName : %p"), this);
	}
}

void ABaseWeapon::OnEquippedTarget(UCombatComponent* combatcomp)
{
	if (GetOwner() != nullptr)
	{
		if (combatcomp != nullptr)
		{
			FName selectedName = (combatcomp->bCombatEnable ? handSocketName : AttachSocketName);

			AttachActor(selectedName);

			combatcomp->SetMainWeapon(this);

			ACharacter* characterPtr = Cast<ACharacter>(GetOwner());
			if (characterPtr)
			{
				USkeletalMeshComponent* skeletalMeshComponent = characterPtr->GetMesh();
				if (skeletalMeshComponent)
				{
					UAnimInstance* animInstance = skeletalMeshComponent->GetAnimInstance();

					if (animInstance != nullptr)
					{
						IAnimInstance_Interface* AnimInterface = Cast<IAnimInstance_Interface>(animInstance);

						if (AnimInterface != nullptr)
						{
							AnimInterface->UpdateCombatType_Implementation(eWeaponType);

							//UE_LOG(LogTemp, Warning, TEXT("%s Send UUpdateCombatType"), *animInstance->GetName());
						}
						else
						{
							//UE_LOG(LogTemp, Warning, TEXT("%s Is do not Have IAnimInstance_Interface"), *skeletalMeshComponent->GetName());
						}
					}
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("skeletalMeshComponent Is Nullptr"));
				}
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Owner Is Not Character"));
			}

			collisionComponent->SetCollisionMesh(GetItemMesh());
			collisionComponent->AddActorToIgnore(GetOwner());
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("selectedName : %p"), this);
	}
}


void ABaseWeapon::OnHitCollisionComponent(FHitResult lastHitStruct)
{	
	AActor* hitActor = lastHitStruct.GetActor();
	ABossApernia* boss = Cast<ABossApernia>(hitActor);
	ADemonSword* demonSword = Cast<ADemonSword>(hitActor);
	AGrux* grux = Cast<AGrux>(hitActor);
	ADogBart* dogBart = Cast<ADogBart>(hitActor);
	ADragonRazorStatue* statue = Cast<ADragonRazorStatue>(hitActor);
	Player = Cast<APlayerOrganism>(GetOwner());
	if (boss && !bHit)
	{		
		GetWorldTimerManager().SetTimer(timerhandle_CoolTimeBossHit, this, &ABaseWeapon::HitCoolTimeSet, 0.01, false);

		if (Player->bBindEnemy)
		{
			boss->BossFallDown();
		}
		boss->BossTakeDamage(10.0f);
		
		bHit = true;
	}	

	if (statue && !bHit)
	{
		GetWorldTimerManager().SetTimer(timerhandle_CoolTimeBossHit, this, &ABaseWeapon::HitCoolTimeSet, 0.1, false);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), swordHitNA, GetActorLocation(), GetActorRotation(), FVector(10.0f));
		statue->TakeDamageD(10.0f);
		bHit = true;

	}

	if (demonSword && !bHit)
	{
		GetWorldTimerManager().SetTimer(timerhandle_CoolTimeBossHit, this, &ABaseWeapon::HitCoolTimeSet, 0.1, false);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), swordHitNA, GetActorLocation(), GetActorRotation(), FVector(10.0f));
		demonSword->SwordTakeDamage(10.0f);
		bHit = true;

	}
	if (grux && !bHit&&grux->gruxDie == false)
	{
		GetWorldTimerManager().SetTimer(timerhandle_CoolTimeBossHit, this, &ABaseWeapon::HitCoolTimeSet, 0.1, false);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), swordHitNA, GetActorLocation(), GetActorRotation(), FVector(10.0f));
		grux->GruxTakeDamage(10.0f);

		grux->ServerRPC_TakeDamage();
		bHit = true;

	}
	if (dogBart && !bHit && dogBart->dogBartDie == false)
	{
		GetWorldTimerManager().SetTimer(timerhandle_CoolTimeBossHit, this, &ABaseWeapon::HitCoolTimeSet, 0.1, false);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), swordHitNA, GetActorLocation(), GetActorRotation(), FVector(10.0f));
		dogBart->DogBartTakeDamage(10.0f);

		dogBart->ServerRPC_TakeDamage();
		bHit = true;

	}
	
	FVector Direction = (EndSwordMark - lastHitStruct.ImpactPoint).GetSafeNormal();
	SwordMarkRotation = Direction.Rotation();

	auto interfaceCheck = Cast<ICombatInterface>(hitActor);

	if (interfaceCheck != nullptr)
	{
		interfaceCheck->CanReceiveDamage_Implementation();

		FVector hitFromDirection = GetOwner()->GetActorForwardVector();

		TSubclassOf<UDamageType> damageTypeClass = {};

		// 데미지를 플레이어에게 넣어주는 함수
		UGameplayStatics::ApplyPointDamage(hitActor, weaponDamage, hitFromDirection, lastHitStruct, GetInstigatorController(), this, damageTypeClass);
	}
}


void ABaseWeapon::SimulateWeaponPhysics()
{
	GetItemMesh()->SetCollisionProfileName(FName("PhysicsActor"));
	GetItemMesh()->SetSimulatePhysics(true);
}

void ABaseWeapon::HitCoolTimeSet()
{

	bHit = false;
}

