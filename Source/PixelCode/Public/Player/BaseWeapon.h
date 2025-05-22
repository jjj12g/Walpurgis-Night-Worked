// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Equipmentable.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */
class UCollisionComponent;
class UCombatComponent;
class UAnimMontage;
class APlayerOrganism;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None = 0				UMETA(DisplayName = "None"),
	LightSword				UMETA(DisplayName = "LightSword"),
	GreatSword				UMETA(DisplayName = "GreatSword"),
	MagicStaff              UMETA(DisplayName = "MagicStaff"),
	Pick					UMETA(DisplayName = "Pick"),
	E_WeaponTypeMax
};


UCLASS()
class PIXELCODE_API ABaseWeapon : public AEquipmentable
{
	GENERATED_BODY()


public:
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NS_HitImpact;

public:

	// Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	FName handSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	EWeaponType			eWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage*			enterCombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage*			exitCombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TArray<UAnimMontage*>	attackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TArray<UAnimMontage*>	dodgeMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")

	UCollisionComponent* collisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCombatComponent* combatComponent;

	UPROPERTY()
	APlayerOrganism* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initialization")
	float weaponDamage;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class UNiagaraSystem* swordHitNA;

	// Functions
	void OnEquipped();

	void OnEquippedTarget(UCombatComponent* combatcomp);

	UFUNCTION()
	void OnHitCollisionComponent(FHitResult lastHitStruct);

	UFUNCTION(BlueprintCallable)
	void SimulateWeaponPhysics();

	FTimerHandle timerhandle_CoolTimeBossHit;

	void HitCoolTimeSet();

	bool bHit = false;

	FVector StartSwordMark;
	FVector EndSwordMark;

	FRotator SwordMarkRotation;



};
