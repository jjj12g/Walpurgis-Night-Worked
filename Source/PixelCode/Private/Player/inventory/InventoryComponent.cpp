// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/inventory/InventoryComponent.h"
#include "Player/inventory/ItemBase.h"
#include "Player/PixelCodeCharacter.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent GetIsReplicated() : %s"), GetIsReplicated() ? TEXT("TRUE") : TEXT("FALSE"));
	if (GetIsReplicated() != true)
	{

		SetIsReplicated(true);
	}

	//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent GetIsReplicated() : %s"), GetIsReplicated() ? TEXT("TRUE") : TEXT("FALSE"));
	if (GetIsReplicated() != true)
	{

		SetIsReplicated(true);
	}
}



void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn) // 포인터가 유효한지 확인
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn)) // 키로 항목찾기, 작동하면 요소유형의 포인터임.
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate([&ItemIn] // 이중포인터
	(const UItemBase* InventoryItem)
		{

			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack(); // ID가 같고 풀로차있지않은지
		}
	))
	{
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight()); // 아이템중량에서 총중량뺀값 중량용량이 50이고 20을운반하면 30이됨, 부동소수점이 나오면 가장가까운정수로 내림함.
	if (WeightMaxAddAmount >= RequestedAddAmount) // 최대중량이 현재중량보다 크거나같다면 요청한 숫자반환
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity; // 데이터를 가져오고 최대스택크기를 가져와서 현재수량을 뺌

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack); // 둘중 더 작은 값 반환
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);// 단일항목 제거
	OnInventoryUpdated.Broadcast();

}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity); // 항목의 양 제거

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove); // 수량설정, 항목을 수량으로표시, 제거할 실제양을 뺀 다음 원하는 양을 제거해서 무게조정

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();// 업데이트하고 

	return ActualAmountToRemove; // 실제로 제거한 양 반환
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity)) // 인벤토리에 넘치지않으면 분할
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

void UInventoryComponent::RefreshInventory()
{
	OnInventoryUpdated.Broadcast();
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
	// 유효한 가중치가 있는지 확인, 용량이없을경우
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item has invalid weight value."), InputItem->TextData.Name)); // 추가하지않음
	}

	// will the item weight overflow weight capacity 추가하면 용량초과
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), InputItem->TextData.Name));
	}

	// adding one more item would overflow slot capacity 항목을 하나 더 추가하면 슬롯용량초과
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->TextData.Name));
	}

	// 다 통과했으면 성공
	AddNewItem(InputItem, 1);

	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString("Successfully added a single {0} to the inventory."), InputItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight())) // 요청한 숫자가 잘못되거나 0에 가까운지 확인
	{
		// 유효하지 않은 항목 데이터
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount; // 인벤토리에 넣기위한 남은자리

	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn); // 아이템 id 비교하여 동일한 종류의 항목인지 확인

	while (ExistingItemStack) // Loop를 사용하는데 null이 들어옴
	{
		// 남은 무게나 스택 둘다 계산
		// 전체 스택을 만들기 위한 전체 스택 금액이 아닌지 확인한다는 것은 다음 전체 스택을 만드는 데 필요한 기존 항목의 수를 계산하는 것.
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute); // 최대가 4고 2가 들어있는 상태 2가 들어올 수 있음, 스택 계산
		// 무게 제한 전체 스택 무게 중 실제로 얼마나 많은 양을 운반할 수 있는지 계산
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack); // 무게계산
 
		// 물품의 남은 양이 무게 용량을 초과하지 않는 한
		if (WeightLimitAddAmount > 0)
		{
			// 기존 품목 스택 수량 및 재고 총 중량 조정
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

			// 분배할 개수를 조정
			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			// 이제 최대 무게에 가까워졌기때문에 루프 계속 실행할필요 x
			if (InventoryTotalWeight >= InventoryWeightCapacity) 
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				// 여러 스택에 항목을 배포하면 이 블록에 도달
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}
		if (AmountToDistribute <= 0) // 마지막으로 배포할 금액이 0보다 작거나 같은경우, 모든것 배포. 위에는 코드는 일부배포
		{
			//기존 스텍에 분산되어서 더이상 배포할 항목이 없으므로
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}
		// 해당 항목의 유효한 부분 스택이 아직 남아있는지 확인
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// 더 이상 부분스택 발견x  새 스택 바로 추가할 수 있는지 확인
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity) // 콘텐츠의 번호 +1이 슬롯 용량보다 작거나 같은경우
	{
		// 따라서 우리는 이미 남은 항목 수량에서 재고 용량에 맞을 수 있는 만큼 추가
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn,AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			// 아직 남은 아이템이 있지만 무게 제한에 도달.
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				// 입력 항목을 조정하고 올바른 수만큼 새 스택을 추가
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				// 부분 스택만 추가되므로 복사본을 만듭니다.
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}
			// 스택의 나머지 전체 추가
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}

	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;
		if (!InputItem->NumericData.bisStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		// handle stakckable 값 추가
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("Successfully added {0} {1} to the inventory."), InitialRequestedAddAmount, InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"), InputItem->TextData.Name, StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0) // 추가된 값이 0보다 작으면 안되지만
		{
			return FItemAddResult::AddedNone(FText::Format(FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."), InputItem->TextData.Name));
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed."));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem;

	

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// 항목이 이미 사본이거나 픽업인 경우
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// 그렇지않은경우
		NewItem = Item->CreateItemCopy(); // 항목 복사본을 생성하고 호출
	}

	NewItem->OwningInventory = this; // 인벤토리에 아이템을 집어넣음
	NewItem->SetQuantity(AmountToAdd);	// 추가할 수량설정

	InventoryContents.Add(NewItem);	// 새 항목 추가
	InventoryTotalWeight += NewItem->GetItemStackWeight();// 가중치 조정
	OnInventoryUpdated.Broadcast(); // 모든 UI요소에 변경사항 적용

}




