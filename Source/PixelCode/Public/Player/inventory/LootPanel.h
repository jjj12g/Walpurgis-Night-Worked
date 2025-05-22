// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootPanel.generated.h"

/**
 * 
 */
 class APlayerOrganism;

UCLASS()
class PIXELCODE_API ULootPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override; // À§Á¬»ý¼ºÀÚ
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	
public:
	UPROPERTY()
	APlayerOrganism* PlayerCharacter;


};
