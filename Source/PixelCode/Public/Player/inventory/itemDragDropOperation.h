// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Player/PixelCodeCharacter.h"
#include "itemDragDropOperation.generated.h"

class UItemBase;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UitemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UItemBase* SourceItem;

	UPROPERTY()
	UInventoryComponent* SourceInventory;
	
};
