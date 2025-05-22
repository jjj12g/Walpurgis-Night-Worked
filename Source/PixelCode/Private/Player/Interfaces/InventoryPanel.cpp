// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interfaces/InventoryPanel.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WrapBox.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Player/PixelCodeCharacter.h"
#include "Player/inventory/InventoryComponent.h"
#include "Player/Inventory/InventoryItemSlot.h"
#include "Player/inventory/itemDragDropOperation.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<APlayerOrganism>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory(); // 인벤토리를 가져오고 인벤토리 가져오기를 기억
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory); // 인벤토리 패널전달 -> 인벤토리 새로고침
			SetInfoText();
		}
	}

}

// 무게 정보와 용량을 설정하기 위한것
void UInventoryPanel::SetInfoText() const
{
	// 무게
	const FString WeightInfoValue
	{
		FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/"
		+ FString::SanitizeFloat(InventoryReference->GetWeightCapacity())
	};


	// 용량
	const FString CapacityInfoValue
	{
		FString::FromInt(InventoryReference->GetInventoryContents().Num()) + "/"
		+ FString::FromInt(InventoryReference->GetSlotsCapacity())
	};

	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));

}

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for (UItemBase* const& InventoryItem : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}

}


bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 인벤토리 안에 드래그앤 드랍
	const UitemDragDropOperation* ItemDragDrop = Cast<UitemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on InventoryPanel."))

		return true;
	}
	return false;
}

void UInventoryPanel::SetTargetCharacter(APlayerOrganism* target)
{
	PlayerCharacter = target;

	InventoryReference = PlayerCharacter->GetInventory(); // 인벤토리를 가져오고 인벤토리 가져오기를 기억
	if (InventoryReference)
	{
		RefreshInventory();
	}
}