// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/inventory/ItemBase.h"
#include <../../../../../../../Source/Runtime/Core/Public/Math/UnrealMathUtility.h>
#include "Player/PixelCodeCharacter.h"
#include "CreateItemData.h"
#include "Player/inventory/InventoryComponent.h"


// 요한 -------=========================================================------------------------------------

TArray<FCraftItem> UItemBase::GetAllCrafting()
{
	return Crafting;
}


FCraftItemInfo UItemBase::GetCraftItemInfoBasedOn(EItemName Name)
{
	return CraftItemData[Name];
}



UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false) // 생성자가 생성되는 동시에 값을 넣어줌.
{


}

void UItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass()); // 새로운 아이템 생성기능, 정적클래스를 얻음.

	ItemCopy->ID = this->ID; // 복사하는 원본을 의미.
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;
	ItemCopy->ItemName = this->ItemName;
	ItemCopy->Buildtypes = this->Buildtypes;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity) 
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bisStackable ? NumericData.MaxStackSize : 1); // 참, 거짓

		if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwningInventory->RemoveSingleInstanceOfItem(this); //수량 0 유지는 의미없어서 그 항목은 제거함.
			}
		}
	}

}

void UItemBase::Use(APixelCodeCharacter* Character)
{


}

void UItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}