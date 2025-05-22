// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UMainMenuWidget;
class UInteractionWidget;
class UPlayerStatWidget;
class UCraftingWidget;
class UBulidWIdget;
struct FInteractableData;


/**
 * HUD는 위젯을 생성하기 완벽한 장소
 */

UCLASS()
class PIXELCODE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible; // 조준모드에 있는 다음메뉴 전환X하기위함 불값


	APlayerHUD();


	void DisplayMenu(); // 표시메뉴기능
	void HideMenu(); // 메뉴숨기기
	void ToggleMenu();


	void ShowInteractionWidget()const;
	void HideInteractionWidget()const;
	void UpdateInteractionWidget(const FInteractableData& InteractableData)const; // 상호작용가능데이터

protected:
	UPROPERTY()
	UMainMenuWidget* MainManuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	virtual void BeginPlay() override;
};
