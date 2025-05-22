// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/ParentItem.h" // 구조체 만든 헤더로 변경
#include "CreateItemData.h"
#include "DataTypes.h"
#include "ItemBase.generated.h"


class UInventoryComponent;
class AParentItem;
/**
 * 
 */
UCLASS()
class PIXELCODE_API UItemBase : public UObject // object로 C++만듬
{
	GENERATED_BODY()
	
 public:
	TArray<FCraftItem> GetAllCrafting();

	FCraftItemInfo GetCraftItemInfoBasedOn(EItemName Name);


	UPROPERTY()
	UInventoryComponent* OwningInventory; // 인벤토리


	UPROPERTY(EditAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemQuality ItemQuality;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EItemName ItemName;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EBuildType Buildtypes;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemAssetData AssetData;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TMap<EItemName, FCraftItemInfo> CraftItemData; // 크리에이트 아이템

	UPROPERTY(VisibleAnywhere, Category = "Item")
	TArray<FCraftItem> Crafting;

	bool bIsCopy;
	bool bIsPickup;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UItemBase();

	void ResetItemFlags(); // 픽업기능 추가

	UItemBase* CreateItemCopy() const; // 아이템 복사

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };// 호출의 오버헤드를 크게 줄여줌, 무 게증가

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return Quantity * NumericData.Weight; };// 동일한 종류 단일무게

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize;};// 전체 항목 스텍, 수량이 최대 스택 크기와 같은지

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity); // 수량 같은지만 체크

	UFUNCTION(Category = "Item")
	virtual void Use(APixelCodeCharacter* Character);// 플레이어 캐릭터 참조인 캐릭터를 가져옴

	UPROPERTY(EditAnywhere, Category = "KYH")
	class AParentItem* Iteminfo;


protected:
	
	bool operator == (const FName& OtherID) const // TArry 비교 가능하게 만듬
	{
		return this->ID == OtherID;
	}
};


