// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/AnimInstance_Interface.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */

class APlayerOrganism;

UCLASS()
class PIXELCODE_API UCharacterAnimInstance : public UAnimInstance, public IAnimInstance_Interface
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType weaponType = EWeaponType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCombatEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDie = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	FVector moveDirection;

	UPROPERTY()
	APlayerOrganism* myCharacter;

	virtual void UpdateCombatType_Implementation(EWeaponType eType) override;
	virtual void UpdateCombatEnabled_Implementation(bool bEnable) override;
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class UCharacterMovementComponent* playerMovementComp;


	//플레이어 E
	UFUNCTION()
	void AnimNotify_blockcounter(); 

	//플레이어 공격 중 회전 제어
	UFUNCTION()
	void AnimNotify_bRotation();

	UFUNCTION()
	void AnimNotify_bEndRotation();

	UFUNCTION()
	void AnimNotify_NSQ();

	UFUNCTION()
	void AnimNotify_NSR();

	UFUNCTION()
	void AnimNotify_BossGroggy();

	UFUNCTION()
	void AnimNotify_notBossGroggy();

	// 플레이어 z 스킬 대쉬
	UFUNCTION()
	void AnimNotify_PlayerZSkillDash();

	UFUNCTION()
	void AnimNotify_FarmFoliage();

	UFUNCTION()
	void AnimNotify_Mine();

	UFUNCTION()
	void AnimNotify_IsJump();

	UFUNCTION()
	void AnimNotify_EndJump();

	UFUNCTION()
	void AnimNotify_AttackBlock();

	UFUNCTION()
	void AnimNotify_CounterCameraShake();

	UFUNCTION()
	void AnimNotify_BIndEnemy();

	UFUNCTION()
	void AnimNotify_SwordQSkillCameraShake();

	UFUNCTION()
	void AnimNotify_SB_SwordInSound();

	UFUNCTION()
	void AnimNotify_SB_SwordOutSound();

	UFUNCTION()
	void AnimNotify_SB_PlayerBaseSound1();

	UFUNCTION()
	void AnimNotify_SB_PlayerBaseSound2();

	UFUNCTION()
	void AnimNotify_SB_PlayerBaseSound3();

	UFUNCTION()
	void AnimNotify_SB_PlayerBaseSound4();

	UFUNCTION()
	void AnimNotify_SB_PlayerBaseSound5();
	
	UFUNCTION()
	void AnimNotify_SB_PlayerRightSwordSound1();

	UFUNCTION()
	void AnimNotify_SB_PlayerRightSwordSound2();

	UFUNCTION()
	void AnimNotify_SB_SwordQ1Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordQ2Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordQ3Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordQ4Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordQ5Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordE1Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordE2Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordE3Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordE4Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordE5Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordR1Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordZ1Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordZ2Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordZ3Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordZ4Sound();

	UFUNCTION()
	void AnimNotify_SB_SwordZ5Sound();


	// 메이지
	UFUNCTION()
	void AnimNotify_mageRightAttack();
	

	UFUNCTION()
	void AnimNotify_mageQAttack();

	UFUNCTION()
	void AnimNotify_mageEAttack();

	UFUNCTION()
	void AnimNotify_mageRAttack();

	UFUNCTION()
	void AnimNotify_bMove();

	UFUNCTION()
	void AnimNotify_bMoveEnd();
};


