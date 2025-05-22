// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "PixelCodeGameInstance.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UPixelCodeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FCharacterStat GetCharacterDataTable(const FString& rowName);

	UFUNCTION(BlueprintCallable)
	FParentItemInfo GetParentItemDataTable(const FString& rowName);

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_characerStatDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_parentItemDataTable;

};
