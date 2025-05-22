// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "CreateItemData.h"
#include "DataTypes.h"
#include "ParentItem.generated.h"

class UDataTable;

UENUM(BlueprintType)
enum class EItemType : uint8	// 아이템 종류, 갑옷, 무기 , 퀘스트아이템 등
{
	Expendable,
	Equipmentable,
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName = "Shield"),
	Spell UMETA(DisplayName = "Spell"),
	Consumable UMETA(DisplayName = "Consumable"),
	Quest UMETA(DisplayName = "Quest"),
	Mundane UMETA(DisplayName = "Mundane")

};

// HJW
UENUM()
enum class EItemQuality : uint8 // 품질, 등급
{
	Shoddy UMETA(DisplayName = "Shoddy"),
	Common UMETA(DisplayName = "Common"),
	Quality UMETA(DisplayName = "Quality"),
	Masterwork UMETA(DisplayName = "Masterwork"),
	Grandmaster UMETA(DisplayName = "Grandmaster")
};

USTRUCT()
struct FItemStatistics // 스텟에 더할값.
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere) // 어디서든 편집하고싶음
	float ArmorRating; // 방어구 등급

	UPROPERTY(EditAnywhere)
	float DamageValue; // 데미지 값

	UPROPERTY(EditAnywhere)
	float RestorationAmount; // 물약같은 소모품

	UPROPERTY(EditAnywhere)
	float SellValue;
};

USTRUCT()
struct  FItemTextData // 텍스쳐 데이터
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;

};


USTRUCT()
struct FItemNumericData // 숫자데이터 무게등 
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool bisStackable;

};

USTRUCT()
struct FItemAssetData // 에셋 데이터, 물건을 집었을때 사운드같은거 넣기도 좋음
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

};

// 구조체
USTRUCT()
struct  FItemData : public FTableRowBase // 행 베이스 상속
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemName ItemName;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EBuildType Buildtypes;

};
// HJW

USTRUCT(BlueprintType)
struct FParentItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	FString itemImagePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	EItemType itemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	int32 numberOfItemsOwned;

	
};

UCLASS()
class PIXELCODE_API AParentItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString itemName = TEXT("Default");

	FParentItemInfo itemInfo;
};