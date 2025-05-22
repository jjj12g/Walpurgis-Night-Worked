// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h" // ?
#include "Player/PlayerOrganism.h"
#include "Player/Interfaces/InteractionInterface.h" // cpp가아닌 헤더에 둬야함.
#include "Player/inventory/ItemBase.h"
#include "PixelCodeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCombatComponent;
class APlayerHUD;
class UInventoryComponent;
class UItemBase;
class AItemStorage;
class UPlayerStatWidget;
class ABuildingVisual;
class ABuilding;
class UNormallyWidget;
class ABaseWeapon;
class UCreateItemData;
class AParentItem;
struct FRecipe;
class ASpawnSwordQSkill;
class ASpawnSwordRSkill;
class UPCodeGameInstance;
class ApixelPlayerState;
class AMyGameModeBase;
class APlayerMageRightAttackSpawnActor;
class APlayerMageLeftAttackSpawnActor;
class APlayerMageQSkillSpawnActor;
class APlayerMageESkillSpawnActor;
class APlayerMageZSkillSpawnActor;
class USoundBase;
class APlayerMageRSkillCastActor;


UENUM()
enum class MyEnum : int8
{
	NM_Standalone,
	NM_DedicatedServer,
	NM_ListenServer,
	NM_Client,
	NM_MAX,
};

// 진원s
USTRUCT()
struct FInteractionData
{	
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	}

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime; // 상호작용 가능항목 포함, 마지막 상호작용 포함, 시간확인

};
// 진원e

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APixelCodeCharacter : public APlayerOrganism
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleCombatAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Pressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_RollandRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Stat;

	// Skilll
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_SkillQ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_SkillE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_SkillR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_SkillZ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Skill_RightMouse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_Weapon; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_Weapon2; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_Weapon3; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_Weapon4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_ExpUp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) 
	UInputAction* IA_StopWidget;

	

public:
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	

	UPROPERTY(EditDefaultsOnly, Category = "MySettings", Replicated)
	class UAnimMontage* AM_DeathMontage;


	// 플레이어 마나, 스테미너 시스템
	float MPRegen = 0.0f;
	float SPRegen = 0.0f;
	float SPRegenTime = 0.0f;

	UPROPERTY(Replicated)
	bool bPoss = false;

	class APickup* Pickup;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KSH)
	class AActor* Rock;

	APixelCodeCharacter();
	

	// 진원 S
	FORCEINLINE bool IsInteracting() const {return GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Interaction); }; // 현재 상호작용중인지 아닌지

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);


	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_DropItem(const FTransform ASpawnTransform, UItemBase* ItemToDrop, int32 RemoveQuantity);

	class AInterfaceTestActor* InterfaceActor;

	UPROPERTY()
	UPlayerStatWidget* statWidget;

	UPROPERTY()
	UNormallyWidget* NormallyWidget;

	class APCodePlayerController* Pc;

	// 구르기
	bool bRoll = false;

	
	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* RollAnim;

	float RollTime = 0;

	// 스킬
	void SkillQ();

	void SkillE();

	void SkillR();

	void SkillZ();

	void SkillRightMouse();

	// 스킬 쿨타임

	bool bQskillCoolTime = false;
	bool bEskillCoolTime = false;
	bool bRskillCoolTime = false;
	bool bZskillCoolTime = false;
	void QskillTime();
	void EskillTime();
	void RskillTime();
	void ZskillTime();

	FTimerHandle QSkillTimer;
	FTimerHandle ESkillTimer;
	FTimerHandle RSkillTimer;
	FTimerHandle ZSkillTimer;

	float QSkillCoolTime = 6;
	float ESkillCoolTime = 10;
	float RSkillCoolTime = 8;
	float ZSkillCoolTime = 20;

	float CurrentQSkillCoolTime = 0;
	float CurrentESkillCoolTime = 0;
	float CurrentRSkillCoolTime = 0;
	float CurrentZSkillCoolTime = 0;

	// 스킬 스폰

	UPROPERTY()
	ASpawnSwordQSkill* SpawnQSkillCollsion;

	UPROPERTY()
	ASpawnSwordRSkill* SpawnRSkillCollsion;

	UPROPERTY()
	APlayerMageRightAttackSpawnActor* mageSpawnRightAttackCollsion;

	UPROPERTY()
	APlayerMageLeftAttackSpawnActor* mageSpawnLeftAttackCollsion;

	UPROPERTY()
	APlayerMageQSkillSpawnActor* mageSpawnQSkillCollsion;

	UPROPERTY()
	APlayerMageESkillSpawnActor* mageSpawnESkillCollsion;

	UPROPERTY()
	APlayerMageRSkillCastActor* mageSpawnRSkillCollsion;

	UPROPERTY()
	APlayerMageZSkillSpawnActor* mageSpawnZSkillCollsion;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ASpawnSwordQSkill> QSkillSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<ASpawnSwordRSkill> RSkillSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<APlayerMageRightAttackSpawnActor> mageRightAttackSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<APlayerMageLeftAttackSpawnActor> mageLeftAttackSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<APlayerMageQSkillSpawnActor> mageQSkillSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<APlayerMageESkillSpawnActor> mageESkillSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<APlayerMageRSkillCastActor> mageRSkillSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<APlayerMageZSkillSpawnActor> mageZSkillSpawn;

	// 검사 스킬스폰
	UFUNCTION(Server, Reliable) 
	void SeverRPC_QSkillSpawn();

	UFUNCTION(NetMulticast, Reliable) 
	void MultiRPC_QSkillSpawn();

	UFUNCTION(Server, Reliable) 
	void SeverRPC_RSkillSpawn();

	UFUNCTION(NetMulticast, Reliable) 
	void MultiRPC_RSkillSpawn();

	// 메이지 스킬 스폰
	UFUNCTION(Server, Reliable) 
	void SeverRPC_mageLeftAttackSpawn();

	UFUNCTION(NetMulticast, Reliable) 
	void MultiRPC_mageLeftAttackSpawn();
	
	UFUNCTION(Server, Reliable) 
	void SeverRPC_mageRightAttackSpawn();
	
	UFUNCTION(NetMulticast, Reliable) 
	void MultiRPC_mageRightAttackSpawn();
	
	UFUNCTION(Server, Reliable) 
	void SeverRPC_mageQSkillSpawn();

	UFUNCTION(NetMulticast, Reliable) 
	void MultiRPC_mageQSkillSpawn();
	
	UFUNCTION(Server, Reliable) 
	void SeverRPC_mageESkillSpawn();

	UFUNCTION(NetMulticast, Reliable) 
	void MultiRPC_mageESkillSpawn();

	UFUNCTION(Server, Reliable) 
	void SeverRPC_mageRSkillSpawn();

	UFUNCTION(NetMulticast, Reliable) 
	void MultiRPC_mageRSkillSpawn();

	UFUNCTION(Server, Reliable) 
	void SeverRPC_mageZSkillSpawn();

	UFUNCTION(NetMulticast, Reliable) 
	void MultiRPC_mageZSkillSpawn();

	FVector CachedDestination;

	void Mousehit();

	void switchWeapon();

	void switchWeapon2();

	void switchWeapon3();

	void switchWeapon4();

	void StopWidget();

	// 사운드
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerSwordOutSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerSwordInSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerSwordBaseAttackSound1;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerSwordBaseAttackSound2;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerSwordBaseAttackSound3;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerSwordBaseAttackSound4;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerSwordBaseAttackSound5;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerSwordRightAttackSound1;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerSwordRightAttackSound2;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerQSound1;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerQSound2;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerQSound3;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerQSound4;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerQSound5;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerESound1;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerESound2;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerESound3;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerESound4;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerESound5;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerRSound1;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerZSound1;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerZSound2;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerZSound3;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerZSound4;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerZSound5;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerJumpSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* PlayerDieSound;

	
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSwordOutSound();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerSwordOutSound();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSwordInSound();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerSwordInSound();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSwordBaseAttackSound1();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerSwordBaseAttackSound1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSwordBaseAttackSound2();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerSwordBaseAttackSound2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSwordBaseAttackSound3();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerSwordBaseAttackSound3();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSwordBaseAttackSound4();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerSwordBaseAttackSound4();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSwordBaseAttackSound5();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerSwordBaseAttackSound5();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSwordRightAttackSound1();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerSwordRightAttackSound1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerSwordRightAttackSound2();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerSwordRightAttackSound2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerQSound1();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerQSound1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerQSound2();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerQSound2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerQSound3();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerQSound3();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerQSound4();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerQSound4();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerQSound5();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerQSound5();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerESound1();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerESound1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerESound2();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerESound2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerESound3();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerESound3();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerESound4();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerESound4();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerESound5();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerESound5();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerRSound1();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerRSound1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerZSound1();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerZSound1();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerZSound2();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerZSound2();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerZSound3();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerZSound3();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerZSound4();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerZSound4();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerZSound5();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerZSound5();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerJumpSound();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerJumpSound();

	
	void Soundcollection();

	// 진원 E
protected:
	// 진원 S
	UPROPERTY()
	APlayerHUD* HUD;

	// 진원 E
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void CharacterJump(const FInputActionValue& Value);

	void LightAttackFunction(const FInputActionValue& Value);

	void ToggleCombatFunction(const FInputActionValue& Value);

	void PlayerRoll(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerRoll();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerRoll();

	void PlayerRun(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerRun();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerRun();

	void PlayerRunEnd(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerRunEnd();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_PlayerRunEnd();

	void PlayerExpUp(const FInputActionValue& Value);

	bool bInventorystate = false;

	virtual void Tick(float DeltaTime) override;

	void PrintInfo();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
		
	// To add mapping context
	virtual void BeginPlay();

	// 진원 s
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFrequecy;

	float InteractionCheckDistance; // 추적이 캐릭터에서 얼마나 멀리 발사될지

	FTimerHandle TimerHandle_Interaction; 

	UPROPERTY(Replicated)
	FInteractionData InteractionData;

	void ToggleMenu();

	void StatMenu();

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable); // 상호작용 가능항목호출, 새 상호작용 가능항목 가져오기
	void NoInteractableFound();// 하지만 찾지못하면 호출X
	void BeginInteract();
	void EndInteract();
	// 진원 e
		
	UFUNCTION(Server, Reliable)
	void ServerRPC_Interact();
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_Interact(const TScriptInterface<IInteractionInterface>& Interactable);

	APlayerOrganism* focusedChar = nullptr;

	APixelCodeCharacter* self = this;

public:
	void Interact();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon")
	TSubclassOf<class ABaseWeapon> defaultWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon")
	TSubclassOf<class ABaseWeapon> axe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon")
	TSubclassOf<class ABaseWeapon> Pick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weapon")
	TSubclassOf<class ABaseWeapon> magicStaff;

	ABaseWeapon* equipment;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// 카메라 속도
    UPROPERTY(EditAnywhere, Category = Camera)
    float CameraLagSpeed;

	FVector CameraLoc;

	AMyGameModeBase* GM;

	UPROPERTY()
	ApixelPlayerState* pixelPlayerState;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ToggleCombat();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastRPC_ToggleCombat();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
    
	void InitMainUI();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Die();

	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Die();

	virtual void DieFunction() override;

	virtual void CreateInventory() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	private:
		FVector camPosition = FVector(-500, 0, 60);
};


