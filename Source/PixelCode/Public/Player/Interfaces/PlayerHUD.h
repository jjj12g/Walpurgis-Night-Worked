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
 * HUD�� ������ �����ϱ� �Ϻ��� ���
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

	bool bIsMenuVisible; // ���ظ�忡 �ִ� �����޴� ��ȯX�ϱ����� �Ұ�


	APlayerHUD();


	void DisplayMenu(); // ǥ�ø޴����
	void HideMenu(); // �޴������
	void ToggleMenu();


	void ShowInteractionWidget()const;
	void HideInteractionWidget()const;
	void UpdateInteractionWidget(const FInteractableData& InteractableData)const; // ��ȣ�ۿ밡�ɵ�����

protected:
	UPROPERTY()
	UMainMenuWidget* MainManuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	virtual void BeginPlay() override;
};
