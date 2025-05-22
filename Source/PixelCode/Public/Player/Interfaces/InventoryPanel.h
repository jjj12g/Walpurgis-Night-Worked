// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class UInventoryComponent;
class APlayerOrganism;
class UInventoryItemSlot;
class UWrapBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION()
	void RefreshInventory(); // 인벤토리 새로 고치는 기능

	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeightInfo;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CapacityInfo;

	UPROPERTY()
	APlayerOrganism* PlayerCharacter;
	UPROPERTY()
	UInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

protected:
	void SetInfoText() const;
	virtual void NativeOnInitialized() override; // 생성자
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override; // 인벤토리 항목을 드롭할 수 있기 때문에


public:
	void SetTargetCharacter(APlayerOrganism* target);
};

