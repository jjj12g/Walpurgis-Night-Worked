// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatInterface.h"
#include "StateComponent.h"
#include "inventory/InventoryComponent.h"
#include "PlayerOrganism.generated.h"

UENUM()
enum class ECharacterType : int8
{
	PlayerCharacter,
	Enemy
};

UENUM()
enum class ECharacterMotionState : int8
{
	Idle,
	ToggleCombat,
	Attack,
	Defence,
	Jump,
	Die
};

class UAnimMontage;
class UCombatComponent;
class ULootPanel;
UCLASS()
class PIXELCODE_API APlayerOrganism : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerOrganism();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString characterName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	bool bDead;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	UStateComponent* stateComp;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCombatComponent* combatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	ECharacterType characterType;
	
	// 피격시 HitReactMontage 재생을 위한 함수
	void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	UAnimMontage* hitReaction;	

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	UAnimMontage* hitFalldownReaction;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings")
	UAnimMontage* AttackCounter;

	class UAnimInstance* AnimInsatnce;

	void GetHit(const FVector& ImpactPoint, bool bFallDown);

	UPROPERTY(Replicated, BlueprintReadWrite)
	ECharacterMotionState motionState = ECharacterMotionState::Idle;

	
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	virtual void ContinueAttack_Implementation() override;
	virtual void ResetCombat_Implementation() override;
	virtual bool CanReceiveDamage_Implementation() override;

	TArray<int32> attackDamageArray;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)override;

	
	FName pelvisBoneName = TEXT("Pelvis");

	UFUNCTION(BlueprintCallable)
	void AttackEvent();

	UFUNCTION(BlueprintCallable)
	void PerformAttack(int32 attackIndex, bool bUseRandom);

	void EnableRagdoll();

	FString GetName() {return characterName;};

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_PerformAttack(UAnimMontage* useMontage);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PerformAttack(UAnimMontage* useMontage);

	UFUNCTION(Server, Reliable)
	void ServerRPC_AmountDamage(float damage);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_AmountDamage(float currentHP);

	UFUNCTION()
	virtual void DieFunction();

	bool bFarmFoliage = false;
	bool bMine = false;

	bool bBindEnemy = false;

	bool SkillZ = false;

	// 캐릭터가 현재 이동 중인지 여부
	bool SkillE = false;

	// Lerp 알파 변수 (보간 제어)
	float dashSkillTime = 0.0f;

	// 플레이어 공격중 회전 막기
	bool bRotation = false;

	// 플레이어 스킬중 다른공격 못하게 설정
	bool bUseSkill = false;
	
	// 플레이어 스킬 나이아가라
	bool bSkillNSQ = false;
	bool bSkillNSR = false;

	bool bBossGroggy = false;

	// 카운터 관련
	bool bAttackBlock = false;

	// 카메라 쉐이크
	bool bSwordQSkillCameraShake = false;

	// 플레이어 사운드
	bool bSwordInSound = false;

	bool bSwordOutSound = false;

	bool bBaseSwordSound1 = false;

	bool bBaseSwordSound2 = false;

	bool bBaseSwordSound3 = false;

	bool bBaseSwordSound4 = false;

	bool bBaseSwordSound5 = false;

	bool bRightSwordSound1 = false;

	bool bRightSwordSound2 = false;

	bool bSwordQ1Sound = false;

	bool bSwordQ2Sound = false;

	bool bSwordQ3Sound = false;

	bool bSwordQ4Sound = false;

	bool bSwordQ5Sound = false;

	bool bSwordE1Sound = false;
	
	bool bSwordE2Sound = false;

	bool bSwordE3Sound = false;

	bool bSwordE4Sound = false;

	bool bSwordE5Sound = false;

	bool bSwordR1Sound = false;

	bool bSwordZ1Sound = false;

	bool bSwordZ2Sound = false;

	bool bSwordZ3Sound = false;
	
	bool bSwordZ4Sound = false;

	bool bSwordZ5Sound = false;

	bool bmageRightAttack = false;

	bool bmageQAttack = false;

	bool bmageEAttack = false;

	bool bmageRAttack = false;

	bool bMove = false;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerHitSound;

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerHitSound();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerHitSound();


	void SlowDownTime(float DilationAmount, float Duration, APlayerController* PlayerController);

	UPROPERTY(EditAnywhere,Category = "MySettings|Varibles")
	TSubclassOf<class UCameraShakeBase> PlayerHitShake_bp;

	UPROPERTY(EditAnywhere,Category = "MySettings|Varibles")
	TSubclassOf<class UCameraShakeBase> PlayerQSkillShake_bp;

	bool bCounterCameraShake = false;


	FTimerHandle timerhandle_CounterShakeTimer;

	void CounterCameraShake();

	bool bIsJump = false;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 10.0f;

	 // 목표 위치 변수
    UPROPERTY(EditAnywhere, Category = "Movement")
    FVector TargetLoc;

	// 삼각형 테스트

	float ZSkillTimer = 0.0f;

	bool bDash = false;

	bool bfirstDash = false;
	bool bsecendDash = false;


    int32 CurrentDashIndex = 0;

    FTimerHandle DashTimerHandle;

	FVector VS;
	FRotator RS;

	FVector CurrentLocation;

	FVector NewLocation;

	FVector firstDashLoc;
	FVector secendDashLoc;

   void SkillZTarget();

	UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed;

	// 캐릭터 위치지정
	void CharcurrentLoc();


    // 카메라 이동 속도 변수
    float CameraMoveSpeed;

    // 카메라 이동 시간 변수
    float CameraMoveTime;


	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; }; // 인벤토리 가져오기

	virtual void CreateInventory();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* DeathSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* hitPaticle;

	UPROPERTY()
	int32 itemRandNums = 0;

	UPROPERTY()
	int32 randItemIndex = 0;

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetItemRandNums(int32 randNums);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SetItemRandNums(int32 randNums);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetItemRandIndex(int32 randIndex);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SetItemRandIndex(int32 randIndex);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnItem(int32 itemIndex);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_SpawnItem(int32 randIndex);
};