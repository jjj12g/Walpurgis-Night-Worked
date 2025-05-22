// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inventory/LootPanel.h"
#include "Player/PixelCodeCharacter.h"
#include "Player/inventory/itemDragDropOperation.h"
#include "Player/inventory/ItemBase.h"
#include "Player/PlayerOrganism.h"

void ULootPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void ULootPanel::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<APlayerOrganism>(GetOwningPlayerPawn()); // 기본적으로 HUD는 플레이어 컨트롤러 및 플레이어와 연결되어있음.
}

// 마우스를 클릭하고 드래그할때 데이터보관
bool ULootPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UitemDragDropOperation* ItemDragDrop = Cast <UitemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDrop->SourceItem)
	{
		return true;
	}
	return false;
}

