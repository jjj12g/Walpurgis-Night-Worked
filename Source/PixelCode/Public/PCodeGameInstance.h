// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/DataTable.h"
#include "OnlineSessionSettings.h"
#include "Player/inventory/ItemBase.h"
#include "Player/inventory/InventoryComponent.h"
#include "PCodeGameInstance.generated.h"

/**
 * 
 */

class UDataTable;





UCLASS()
class PIXELCODE_API UPCodeGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
		virtual void Init() override;


	// -------------------------------------------------- 진원
	UFUNCTION(BlueprintCallable)
	FCharacterStat GetCharacterDataTable(const FString& rowName);

	UFUNCTION(BlueprintCallable)
	FParentItemInfo GetParentItemDataTable(const FString& rowName);
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_characerStatDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_parentItemDataTable;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_CharacterskinDatatable;

	// -------------------------------------------------- 진원


};

