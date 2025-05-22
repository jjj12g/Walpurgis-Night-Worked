// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterAnimInstance.h"
#include "Player/PlayerOrganism.h"



void UCharacterAnimInstance::UpdateCombatType_Implementation(EWeaponType eType)
{
	weaponType = eType;
}

void UCharacterAnimInstance::UpdateCombatEnabled_Implementation(bool bEnable)
{
	bCombatEnable = bEnable;
}

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	myCharacter = Cast<APlayerOrganism>(GetOwningActor());


	if (myCharacter != nullptr)
	{
		playerMovementComp = myCharacter->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	if (myCharacter == nullptr)
		return;

	if (myCharacter != nullptr)
	{
		bDie = myCharacter->bDead;
	}
}

void UCharacterAnimInstance::AnimNotify_blockcounter()
{
	if (myCharacter != nullptr)
	{
		myCharacter->SkillE = true;
		myCharacter->CharcurrentLoc();
	}

}

void UCharacterAnimInstance::AnimNotify_bRotation()
{
	if (myCharacter != nullptr)
	{
			myCharacter->bRotation = true;
			myCharacter->bUseSkill = true;
	}
}

void UCharacterAnimInstance::AnimNotify_bEndRotation()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bRotation = false;
		myCharacter->bUseSkill = false;
	}
}

void UCharacterAnimInstance::AnimNotify_NSQ()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSkillNSQ = true;
	}
}

void UCharacterAnimInstance::AnimNotify_NSR()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSkillNSR = true;
	}
}



void UCharacterAnimInstance::AnimNotify_BossGroggy()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bBossGroggy = true;
	}
}

void UCharacterAnimInstance::AnimNotify_notBossGroggy()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bBossGroggy = false;
	}
}

void UCharacterAnimInstance::AnimNotify_PlayerZSkillDash()
{
	if (myCharacter != nullptr)
	{
		myCharacter->SkillZ = true;
		myCharacter->CharcurrentLoc();
	}

}

void UCharacterAnimInstance::AnimNotify_FarmFoliage()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bFarmFoliage = true;
	}
}

void UCharacterAnimInstance::AnimNotify_Mine()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bMine = true;
	}
}

void UCharacterAnimInstance::AnimNotify_IsJump()
{
	if (myCharacter != nullptr)
	{
		
		myCharacter->bIsJump = true;
	}
}

void UCharacterAnimInstance::AnimNotify_EndJump()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bIsJump = false;
	}

}

void UCharacterAnimInstance::AnimNotify_AttackBlock()
{
	if (myCharacter != nullptr)
	{ 
		if (myCharacter->bAttackBlock == false)
		{
			myCharacter->bAttackBlock = true;
		}
		else
		{
			myCharacter->bAttackBlock = false;
		}
	}
}

void UCharacterAnimInstance::AnimNotify_CounterCameraShake()
{
	if (myCharacter != nullptr)
	{
		if (myCharacter->bCounterCameraShake == false)
		{
			//myCharacter->bAttackBlock = true;
		}
	}
}

void UCharacterAnimInstance::AnimNotify_BIndEnemy()
{
	if (myCharacter != nullptr)
	{
		if (!myCharacter->bBindEnemy)
		{ 
			myCharacter->bBindEnemy = true;
		}
		else
		{
			myCharacter->bBindEnemy = false;
		}
	}
}

void UCharacterAnimInstance::AnimNotify_SwordQSkillCameraShake()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordQSkillCameraShake = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordInSound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordInSound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordOutSound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordOutSound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_PlayerBaseSound1()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bBaseSwordSound1 = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_PlayerBaseSound2()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bBaseSwordSound2 = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_PlayerBaseSound3()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bBaseSwordSound3 = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_PlayerBaseSound4()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bBaseSwordSound4 = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_PlayerBaseSound5()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bBaseSwordSound5 = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_PlayerRightSwordSound1()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bRightSwordSound1 = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_PlayerRightSwordSound2()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bRightSwordSound2 = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordQ1Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordQ1Sound = true;
	}

}

void UCharacterAnimInstance::AnimNotify_SB_SwordQ2Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordQ2Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordQ3Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordQ3Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordQ4Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordQ4Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordQ5Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordQ5Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordE1Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordE1Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordE2Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordE2Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordE3Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordE3Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordE4Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordE4Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordE5Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordE5Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordR1Sound()
{	
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordR1Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordZ1Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordZ1Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordZ2Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordZ2Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordZ3Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordZ3Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordZ4Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordZ4Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_SB_SwordZ5Sound()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bSwordZ5Sound = true;
	}
}

void UCharacterAnimInstance::AnimNotify_mageRightAttack()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bmageRightAttack = true;
	}
}

void UCharacterAnimInstance::AnimNotify_mageQAttack()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bmageQAttack = true;
	}
}

void UCharacterAnimInstance::AnimNotify_mageEAttack()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bmageEAttack = true;
	}
}

void UCharacterAnimInstance::AnimNotify_mageRAttack()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bmageRAttack = true;
	}
}

void UCharacterAnimInstance::AnimNotify_bMove()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bMove = true;
	}
}

void UCharacterAnimInstance::AnimNotify_bMoveEnd()
{
	if (myCharacter != nullptr)
	{
		myCharacter->bMove = false;
	}
}

