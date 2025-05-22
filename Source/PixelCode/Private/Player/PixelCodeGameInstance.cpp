// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PixelCodeGameInstance.h"
#include "Player/StateComponent.h"
#include "Player/ParentItem.h"
#include <../../../../../../../Source/Runtime/Core/Public/Templates/SharedPointer.h>
#include <../../../../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h>

FCharacterStat UPixelCodeGameInstance::GetCharacterDataTable(const FString& rowName)
{
	if (dt_characerStatDataTable != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("gameins"));
		FString errorText;
		FCharacterStat* characerStatInfo = dt_characerStatDataTable->FindRow<FCharacterStat>(FName(rowName), errorText);

		if (characerStatInfo != nullptr)
		{
			return *characerStatInfo;
		}
		else
		{
			return FCharacterStat();
		}
	}

	return FCharacterStat();
}

FParentItemInfo UPixelCodeGameInstance::GetParentItemDataTable(const FString& rowName)
{
	if (dt_parentItemDataTable != nullptr)
	{
		FString errorText;
		FParentItemInfo* parentItemInfo = dt_parentItemDataTable->FindRow<FParentItemInfo>(FName(rowName), errorText);

		if (parentItemInfo != nullptr)
		{
			return *parentItemInfo;
		}
		else
		{
			return FParentItemInfo();
		}
	}

	return FParentItemInfo();
}



