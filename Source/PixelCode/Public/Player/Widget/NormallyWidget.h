// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NormallyWidget.generated.h"

class UProgressBar;
class UStateComponent;
class ApixelPlayerState;
class APixelCodeCharacter;
class APCodePlayerController;
class UImage;
class UMaterialInstance;
class UMaterialInstanceDynamic;
class UCanvasPanel;
class UButton;
class AMyGameModeBase;
class UTextBlock;
class APlayerController;
/**
 * 
 */
UCLASS()
class PIXELCODE_API UNormallyWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_HP;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_MP;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_SP;

	UPROPERTY(EditDefaultsOnly, Category = "Stat", meta = (BindWidget))
	UProgressBar* PB_Exp;

	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (BindWidget))
	UTextBlock* TB_LEVEL;

	FString LEVEL;

	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	UStateComponent* statComp;

	UPROPERTY(EditDefaultsOnly, Category = "GM")
	AMyGameModeBase* GM;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	APixelCodeCharacter* Player;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	APCodePlayerController* PlayerController;

	//APlayerController* Pc;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_QSkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_ESkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_RSkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_ZSkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_mageQSkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_mageESkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_mageRSkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "SKillCollTime", meta = (BindWidget))
	UImage* BP_mageZSkillbar;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn", meta = (BindWidget))
	UCanvasPanel* CP_GameOverUI;

	UPROPERTY(EditDefaultsOnly, Category = "StopWidget", meta = (BindWidget))
	UCanvasPanel* CP_StopWidget;

	UPROPERTY(EditDefaultsOnly, Category = "StopWidget", meta = (BindWidget))
	UCanvasPanel* CP_PlayerBaseState;

	UPROPERTY(EditDefaultsOnly, Category = "StopWidget", meta = (BindWidget))
	UCanvasPanel* CP_PlayerSwordSkill;

	UPROPERTY(EditDefaultsOnly, Category = "StopWidget", meta = (BindWidget))
	UCanvasPanel* CP_PlayerMageSkill;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn", meta = (BindWidget))
	UButton* BTN_Respawn;

	UPROPERTY(EditDefaultsOnly, Category = "Respawn", meta = (BindWidget))
	UButton* BTN_Quit;

	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UWidgetSwitcher* SwitcherUI;

	UPROPERTY(EditDefaultsOnly, Meta=(BindWidget))
	class UWidgetSwitcher* SkillSwitcherUI;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstance* MI_RoundProgressbar;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterial* BaseMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* QDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* EDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* RDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* ZDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* mageQDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* mageEDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* mageRDynamicMaterial;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* mageZDynamicMaterial;

	void firstUpdate(UStateComponent* PlayerStateComp);
	void firstStatedate(ApixelPlayerState* Ps);
	
	ApixelPlayerState* PlayerState;

	void currentStatUpdate(UStateComponent* PlayerStateComp);
	void currentExpUpdate(ApixelPlayerState* Ps);
	void currentLevelUpdate(ApixelPlayerState* Ps);
	
	void QSetPercent();
	void ESetPercent();
	void RSetPercent();
	void ZSetPercent();

	void mageQSetPercent();
	void mageESetPercent();
	void mageRSetPercent();
	void mageZSetPercent();

	UFUNCTION()
	void OnMyButtonRespawn();

	UFUNCTION()
	void OnMyButtonQuit();

	void SetActiveGameOverUI(bool value);
	void SetActiveStopWidgetUI(bool value);

	void SetBaseSkillWidget(bool value);
	void SetSwordSkillWidget(bool value);
	void SetMageSkillWidget(bool value);

	bool bPlayerDie = false;

};
