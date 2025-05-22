// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/World/Pickup.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>
#include "Player/inventory/ItemBase.h"
#include "Player/PixelCodeCharacter.h"
#include "Player/Inventory/InventoryComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;


	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);

	ItemDataTable = ConstructorHelpers::FObjectFinder<UDataTable>(TEXT("/Script/Engine.DataTable'/Game/YoHan/DataTable/itembase_-_itembase.itembase_-_itembase'")).Object;

}


void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity); // 수량설정
}


void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 픽업 아이템을 초기화
void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone()) // 없는 아이템인지 확인 ,비어있지않음
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		// 아이템 참조가 없으면 새로 생성
		if (ItemReference == nullptr)
		{
			ItemReference = NewObject<UItemBase>(this, BaseClass);
		}

		// 아이템 데이터로 ItemReference를 초기화
		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;
		ItemReference->ItemName = ItemData->ItemName;
		ItemReference->Buildtypes = ItemData->Buildtypes;

		// 수량 설정
		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		// 메시에 아이템 데이터의 메쉬를 설정
		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		// 상호작용 데이터 업데이트
		UpdateInteractableData();
	}
}


void APickup::NetMulticastRPC_InitializeDrop_Implementation(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight(); // 항목드롭설정
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh); // 드롭할 항목데이터 구조메시확인

	UpdateInteractableData();
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InstanceInteractableData.ItemName = ItemReference->ItemName;
	InteractableData = InstanceInteractableData;


}

void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}

}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(APixelCodeCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter); // 픽업요청, 플레이어캐릭터 전달
	}
}

void APickup::TakePickup(const APlayerOrganism* Taker)
{
	if (!IsPendingKillPending()) // 삭제 대기 중인지 확인
	{
		if (ItemReference)
		{
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				// 플레이어 인벤토리에 항목을 추가한 다음
				 
				// 인벤토리에 아이템 추가 및 결과 처리
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch (AddResult.operationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break; // 아무것도하지않음
				case EItemAddResult::IAR_PartialAmountItemAdded:
				{
					UpdateInteractableData(); // 픽업수량업데이트

					auto charCheck = Cast<APixelCodeCharacter>(Taker);
					if (charCheck != nullptr)
					{
						// 위젯업데이트
						charCheck->UpdateInteractionWidget();
					}
					break;
				}
				case EItemAddResult::IAR_AllItemAdded:
					Destroy();  //아이템이 모두 추가되면 픽업 객체 삭제
					break;
				default:
					break;
				}
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				//UE_LOG(LogTemp, Warning, TEXT("Player inventory conponent is null!!")); // 인벤토리가 널
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somenow null!!")); // 픽업이 널
		}
	}

}


void APickup::SetInput(const APlayerOrganism* Taker)
{
	if (ItemDataTable)
	{
		InitializePickup(UItemBase::StaticClass(), ItemQuantity);
		TakePickup(Taker);
	}
}

FInteractableData APickup::GetItemInfo()
{
	return FInteractableData();
}





