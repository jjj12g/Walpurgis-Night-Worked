// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerOrganism.h"
#include <Engine/DamageEvents.h>
#include <Kismet/KismetSystemLibrary.h>
#include "Animation/AnimMontage.h"
#include "Player/BaseWeapon.h"
#include "Player/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
//#include "LootPanel.h"
#include "Player/inventory/InventoryComponent.h"
#include "Player/Interfaces/InventoryPanel.h"
#include "Player/World/Pickup.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <Animation/AnimInstance.h>
#include <../../../../../../../Source/Runtime/Core/Public/UObject/NameTypes.h>



// Sets default values
APlayerOrganism::APlayerOrganism()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	stateComp = CreateDefaultSubobject<UStateComponent>(TEXT("StateComp"));

	combatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));

	characterName = TEXT("Player");

	CreateInventory();

	MoveSpeed = 1000.0f;
	dashSkillTime = 0.0f;

	CameraMoveSpeed = 5.0f; 
	CameraMoveTime = 0.5f;  

}

// Called when the game starts or when spawned
void APlayerOrganism::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	stateComp->InitStat();
	stateComp->UpdateStat();
	stateComp->dieDelegate.BindUFunction(this, FName("DieFunction"));
	
}

// Called every frame
void APlayerOrganism::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SkillZ)
	{
		if (bDash)
		{
			dashSkillTime += DeltaTime;
			if (bfirstDash)
			{
				NewLocation = FMath::Lerp(VS, firstDashLoc, FMath::Clamp(dashSkillTime*InterpSpeed, 0.0f, 1.0f));
				SetActorLocation(NewLocation);
			}
			else if (bsecendDash)
			{
				NewLocation = FMath::Lerp(firstDashLoc, secendDashLoc, FMath::Clamp(dashSkillTime*InterpSpeed, 0.0f, 1.0f));
				SetActorLocation(NewLocation);
			}

			if (dashSkillTime >= 0.6f)
			{
				bDash = false;
				bfirstDash = false;
				bsecendDash = false;
				SkillZTarget();
				dashSkillTime = 0.0f;
			}
		}
	}

	if (SkillE)
	{
		// 선형 보간을 사용하여 캐릭터 위치 업데이트
		CurrentLocation = GetActorLocation();
		NewLocation = FMath::Lerp(CurrentLocation, TargetLoc, dashSkillTime);
		SetActorLocation(NewLocation);

		// 이동 속도와 델타 타임을 기반으로 Lerp 알파 값을 증가
		float LerpDelta = MoveSpeed * DeltaTime / FVector::Dist(CurrentLocation, TargetLoc);
		dashSkillTime = FMath::Clamp(dashSkillTime + LerpDelta, 0.0f, 1.0f);


		// 목표 위치에 도달했는지 확인
		if (dashSkillTime >= 1.0f)
		{
			dashSkillTime = 0.0f;
			SkillE = false;
		}
	}

	if (bCounterCameraShake)
	{
		APlayerController* APc = Cast<APlayerController>(this->GetController());
		if (APc != nullptr)
		{
			APc->ClientStartCameraShake(PlayerHitShake_bp);
		}
		bCounterCameraShake = false;
	}

	if (bSwordQSkillCameraShake)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PlayerShake"));
		APlayerController* APc = Cast<APlayerController>(this->GetController());
		if (APc != nullptr)
		{
			//UE_LOG(LogTemp,Warning,TEXT("PlayerShake1"));
			APc->ClientStartCameraShake(PlayerQSkillShake_bp);
		}
		bSwordQSkillCameraShake = false;
	}

}


// Called to bind functionality to input
void APlayerOrganism::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

}

float APlayerOrganism::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bAttackBlock)
	{
		bAttackBlock = false;
 		APlayerController* APc = Cast<APlayerController>(this->GetController());
 		if (APc != nullptr)
 		{
 			SlowDownTime(0.1f, 0.05f, APc);
 		}
		GetWorldTimerManager().SetTimer(timerhandle_CounterShakeTimer, this, &APlayerOrganism::CounterCameraShake, 0.9, false);
		AnimInsatnce = GetMesh()->GetAnimInstance();
		if (AnimInsatnce && AttackCounter != nullptr)
		{ 
			AnimInsatnce->Montage_Play(AttackCounter);
			
		}
		return 0.0f;
	}

	float temp = stateComp->AddStatePoint(HP, -DamageAmount);

	

	if (PlayerHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerHitSound, GetActorLocation());
	}

	if (hitReaction != nullptr)
	{
		
	}
	// 디버그
	if (0)
	{
		if (EventInstigator != nullptr)
		{
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("EventInstigator : %s"), *EventInstigator->GetActorNameOrLabel()));
		}

		if (DamageCauser != nullptr)
		{
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Attack By : %s"), *DamageCauser->GetActorNameOrLabel()));
		}
	}
	
	

	return 0.0f;
}


void APlayerOrganism::GetHit(const FVector& ImpactPoint, bool bFallDown)
{
	if (!bUseSkill)
	{ 
		return;
	}
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 8.f, 24, FColor::Orange, false, 5.f);

	const FVector Forward = GetActorForwardVector();

	// 적의 액터 위치 Z에 대한 더 낮은 충격지점
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	// 충격 지점 , 0으로 나뉘는걸 방지하기위해 GetSafeNormal() 사용
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward와 ToHit사이의 값 구하기 -> Dot Product
	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	//(세타)의 역코사인(아크 - 코사인)을 취하여 세타를 구하기
	// 아크 코사인에 코사인 세타전달
	double Theta = FMath::Acos(CosTheta);
	// 라디안을 각도로 변환
	// Theta를 도 단위의 각도로 재지정
	Theta = FMath::RadiansToDegrees(Theta);

	// 만약 벡터가 아래를 가르키면 세타 포인트는 음수여야함.
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
		//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);
	}
		if (hitPaticle != nullptr)
		{
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitPaticle, GetActorLocation() + CrossProduct * 100.f);
		}
		if (bFallDown)
		{
			AnimInsatnce = GetMesh()->GetAnimInstance();
			if (AnimInsatnce && hitFalldownReaction != nullptr)
			{
				if (Theta >= -45.f && Theta < 45.f)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Front"));
				}
				else if (Theta >= -135.f && Theta < -45.f)
				{
					SetActorRotation(GetActorRotation()-FRotator(0,-270,0));
					//UE_LOG(LogTemp, Warning, TEXT("Left"));
				}
				else if (Theta >= 45.f && Theta < 135.f)
				{
					SetActorRotation(GetActorRotation() - FRotator(0,-90,0));
					//UE_LOG(LogTemp, Warning, TEXT("Right"));
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("Back"));
					SetActorRotation(GetActorRotation() - FRotator(0, -180, 0));
				}
				
				AnimInsatnce->Montage_Play(hitFalldownReaction);
				return;
			}
		}
		else
		{ 

		FName Section("HitBack");

		if (Theta >= -45.f && Theta < 45.f)
		{
			Section = FName("HitForward");
			//UE_LOG(LogTemp, Warning, TEXT("Front"));
		}
		else if (Theta >= -135.f && Theta < -45.f)
		{
			Section = FName("HitLeft");
			//UE_LOG(LogTemp, Warning, TEXT("Left"));
		}
		else if (Theta >= 45.f && Theta < 135.f)
		{
			Section = FName("HitRight");
			//UE_LOG(LogTemp, Warning, TEXT("Right"));
		}
		else
		{
		//UE_LOG(LogTemp, Warning, TEXT("Back"));
		}

		PlayHitReactMontage(Section);
		}
}

void APlayerOrganism::PlayHitReactMontage(const FName& SectionName)
{
	AnimInsatnce = GetMesh()->GetAnimInstance();
	if (AnimInsatnce && hitReaction)
	{
		AnimInsatnce->Montage_Play(hitReaction);
		AnimInsatnce->Montage_JumpToSection(SectionName, hitReaction);
	}
}

void APlayerOrganism::ContinueAttack_Implementation()
{
	if (combatComponent == nullptr) return;

	combatComponent->bAttacking = false;

	if (combatComponent->bAttackSaved)
	{
		combatComponent->bAttackSaved = false;

		AttackEvent();
	}
}

void APlayerOrganism::ResetCombat_Implementation()
{
	motionState = ECharacterMotionState::Idle;
	combatComponent->attackCount = 0;
}

bool APlayerOrganism::CanReceiveDamage_Implementation()
{
	return !bDead;
}

void APlayerOrganism::AttackEvent()
{
	if (combatComponent == nullptr) return;

	if (motionState == ECharacterMotionState::Idle || motionState == ECharacterMotionState::Attack)
	{
		PerformAttack(combatComponent->attackCount, false);
	}
}

void APlayerOrganism::PerformAttack(int32 attackIndex, bool bUseRandom) // 이거의 인덱스값을 바꿔주면 애니메이션이 다른게 재생됨. 불값이 트루가 되면 랜덤 공격 애니메이션 재생.
{
	ABaseWeapon* mainWeapon = nullptr;
	UAnimMontage* useMontage = nullptr;

	if (combatComponent)
	{
		mainWeapon = combatComponent->GetMainWeapon();

		if (mainWeapon != nullptr)
		{
			if (mainWeapon->eWeaponType == EWeaponType::GreatSword) // 도끼
			{
				useMontage = mainWeapon->attackMontages[0];
			}
			else if (mainWeapon->eWeaponType == EWeaponType::Pick) // 곡괭이
			{
				useMontage = mainWeapon->attackMontages[0];
			}
			else
			{
				// 랜덤 인덱스 생성
				int32 montagesSize = mainWeapon->attackMontages.Num();
				int32 randIndex = FMath::RandRange(0, montagesSize - 1);

				useMontage = bUseRandom ? mainWeapon->attackMontages[randIndex] : mainWeapon->attackMontages[attackIndex];
			}
			

			if (IsValid(useMontage))
			{
				ServerRPC_PerformAttack(useMontage);
			}
		}
	}
}

void APlayerOrganism::EnableRagdoll()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None, 0);

		if (GetCapsuleComponent())
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

			FAttachmentTransformRules attachTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);

			if (GetMesh())
			{
				GetMesh()->SetCollisionProfileName(TEXT("ragdoll"));
				GetMesh()->SetAllBodiesBelowSimulatePhysics(pelvisBoneName, true, true);
				GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(pelvisBoneName, 1.0f, false, true);
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("AEromCharacter : %d"), __LINE__);
	}
}

void APlayerOrganism::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerOrganism, stateComp);
	DOREPLIFETIME(APlayerOrganism, combatComponent);
	DOREPLIFETIME(APlayerOrganism, motionState);
	DOREPLIFETIME(APlayerOrganism, bDead);
}

void APlayerOrganism::ServerRPC_PerformAttack_Implementation(UAnimMontage* useMontage)
{
	ABaseWeapon* mainWeapon = combatComponent->GetMainWeapon();

	combatComponent->bAttacking = true;

	motionState = ECharacterMotionState::Attack;

	// 여기나 다른데서 캐릭터 스탯 및 무기 데미지 등 적용
	mainWeapon->weaponDamage = mainWeapon->weaponDamage;

	// 카운트 증가

	combatComponent->attackCount++;
	if (combatComponent->attackCount >= 5)
	{
		combatComponent->attackCount = 0;
	}

	if (mainWeapon->eWeaponType == EWeaponType::MagicStaff)
	{
		combatComponent->attackCount = 0;
	}

	

	int32 montageLastIndex = mainWeapon->attackMontages.Num() - 1;

	// 카운트 초기화
	if (combatComponent->attackCount > montageLastIndex)
	{
		combatComponent->attackCount = 0;
	}

	NetMulticastRPC_PerformAttack(useMontage);
}

void APlayerOrganism::NetMulticastRPC_PerformAttack_Implementation(UAnimMontage* useMontage)
{
	float attackAnimTime = PlayAnimMontage(useMontage);

	FTimerHandle handler;
	GetWorldTimerManager().SetTimer(handler, [&]() {

		combatComponent->bAttacking = false;
		motionState = ECharacterMotionState::Idle;

		GetWorldTimerManager().ClearTimer(handler);

		}, 1.0f, false, attackAnimTime);
}

void APlayerOrganism::ServerRPC_AmountDamage_Implementation(float damage)
{
	if (stateComp != nullptr)
	{
		stateComp->AddStatePoint(HP, -damage);
	}
}

void APlayerOrganism::NetMulticastRPC_AmountDamage_Implementation(float damage)
{
	if (stateComp != nullptr)
	{
		stateComp->AddStatePoint(HP, -damage);
	}
}

void APlayerOrganism::DieFunction()
{

	bDead = true;

}

void APlayerOrganism::ServerRPC_PlayerHitSound_Implementation()
{
	NetMulticastRPC_PlayerHitSound();
}

void APlayerOrganism::NetMulticastRPC_PlayerHitSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlayerHitSound, GetActorLocation());
}

void APlayerOrganism::SlowDownTime(float DilationAmount, float Duration, APlayerController* PlayerController)
{

	UGameplayStatics::SetGlobalTimeDilation(PlayerController, DilationAmount);

	// 지정된 시간 후 시간 감소 해제
	FTimerHandle Handle;
	PlayerController->GetWorldTimerManager().SetTimer(Handle, [PlayerController]() {
		UGameplayStatics::SetGlobalTimeDilation(PlayerController, 1.0f); // 시간 감소 해제
		}, Duration, false);
	
}

void APlayerOrganism::CounterCameraShake()
{
	APlayerController* APc = Cast<APlayerController>(this->GetController());
	if (APc != nullptr)
	{
		APc->ClientStartCameraShake(PlayerHitShake_bp);
		//SlowDownTime(0.1f, 0.05f, APc);
	}
}



void APlayerOrganism::SkillZTarget()
{
	if (CurrentDashIndex == 2) 
	{
		SetActorLocation(VS);
		SetActorRotation(RS);
		dashSkillTime = 0.0f;
		CurrentDashIndex = 0;
		return;
	}

	bDash = true;

	if (CurrentDashIndex == 0)
	{
		bfirstDash = true;
		firstDashLoc = GetActorLocation() + ((GetActorForwardVector() + GetActorRightVector() * 0.5f) * 500);
		
		FQuat CurrentQuatRotation = FQuat(GetActorRotation()); // 현재 오브젝트의 로테이션을 쿼터니언으로 변환
		FRotator RelativeRotation(0.f, 250.f, 0.f); // 상대적으로 회전할 양 설정

		// 상대 회전 적용
		FQuat RelativeQuatRotation = FQuat(RelativeRotation);
		FQuat NewQuatRotation = CurrentQuatRotation * RelativeQuatRotation;

		FRotator NewRotation = NewQuatRotation.Rotator();
		SetActorRotation(NewRotation); // 새로운 회전 설정
	}
	else if (CurrentDashIndex == 1)
	{
		bsecendDash = true;
		TargetLoc = GetActorLocation() + (GetActorForwardVector() * 500);
		secendDashLoc = GetActorLocation() + ((GetActorForwardVector() + GetActorRightVector() * 0.5f) * 500);
		

		FQuat CurrentQuatRotation = FQuat(GetActorRotation()); // 현재 오브젝트의 로테이션을 쿼터니언으로 변환
		FRotator RelativeRotation(0.f, -90.0f, 0.f); // 상대적으로 회전할 양 설정

		// 상대 회전 적용
		FQuat RelativeQuatRotation = FQuat(RelativeRotation);
		FQuat NewQuatRotation = CurrentQuatRotation * RelativeQuatRotation;

		FRotator NewRotation = NewQuatRotation.Rotator();
		SetActorRotation(NewRotation); // 새로운 회전 설정
	}

	FVector PlayerLocation = GetActorLocation();
	FString LocationString = PlayerLocation.ToString();

	CurrentDashIndex++;
}

void APlayerOrganism::CharcurrentLoc()
{
	if (SkillZ)
	{
		VS = GetActorLocation();
		RS = GetActorRotation();
		SkillZTarget();
	
		FVector PlayerLocation = GetActorLocation();
		FString LocationString = PlayerLocation.ToString();
	}

	if (SkillE)
	{ 
		TargetLoc = GetActorLocation() + GetActorForwardVector() * 700;
	}
	
}

void APlayerOrganism::CreateInventory()
{
	if (PlayerInventory == nullptr)
	{
		PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	}

	PlayerInventory->SetSlotsCapacity(20); //인벤토리 슬롯 20개생성
	PlayerInventory->SetWeightCapacity(50.0f); // 무게용량 50설정
}


void APlayerOrganism::ServerRPC_SetItemRandNums_Implementation(int32 randNums)
{
	itemRandNums = randNums;

	NetMulticastRPC_SetItemRandNums(randNums);
}

void APlayerOrganism::NetMulticastRPC_SetItemRandNums_Implementation(int32 randNums)
{
	itemRandNums = randNums;
}

void APlayerOrganism::ServerRPC_SetItemRandIndex_Implementation(int32 randIndex)
{
	randItemIndex = randIndex;

	NetMulticastRPC_SetItemRandIndex(randIndex);
}

void APlayerOrganism::NetMulticastRPC_SetItemRandIndex_Implementation(int32 randIndex)
{
	randItemIndex = randIndex;
}

void APlayerOrganism::ServerRPC_SpawnItem_Implementation(int32 itemIndex)
{
	NetMulticastRPC_SpawnItem(itemIndex);
}

void APlayerOrganism::NetMulticastRPC_SpawnItem_Implementation(int32 randIndex)
{
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), FTransform(), spawnParam);
	FString strName = FString::Printf(TEXT("test_%03d"), randItemIndex);
	pickup->DesiredItemID = FName(*strName);
	pickup->SetInput(this);
}

