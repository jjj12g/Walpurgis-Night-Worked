// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CreateItemData.h"
#include "InteractionInterface.generated.h"


class APixelCodeCharacter; // 클래스전방선언, 다른곳에서 정의 될 것임을 알림
class UItemBase;
UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisPlayName = "PickUp"),
	NonPlayerCharacter UMETA(DisPlayName = "NonPlayerCharacter"),
	Device UMETA(DisPlayName = "Device"),
	Toggle UMETA(DisPlayName = "Toggle"),
	Container UMETA(DisPlayName = "Container")
};


USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() :
	InteractableType(EInteractableType::Pickup),
	Name(FText::GetEmpty()), // FText 빈문자 다시반환
	Action(FText::GetEmpty()),
	Quantity(0), // 아이템갯수
	InteractionDuration(0.0f)// 상호작용 시간
	{

	}; // 생성자가 실행될 때 여기에서 첫 번째 작업을 수행

	// 상호작용 가능한 데이터 자체의 복사본을 가져옴.
	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType; // 상호작용 가능유형
	
	UPROPERTY(EditInstanceOnly)
	FText Name;
	
	UPROPERTY(EditInstanceOnly)
	FText Action;
	
	// used Only for Pickups
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	UPROPERTY(EditAnywhere, Category = "Item")
	EItemName ItemName;

	// used for things like valves, doors, etc, that require an interaction timer
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration; // 상호작용 가능시간
};


// 상호작용 인터페이스
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface // 맨밑 UE인터페이스로 제작
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIXELCODE_API IInteractionInterface
{
	GENERATED_BODY()

public:
	virtual void BeginFocus(); // 함수가 하위 클래스에 의해 재정의 될 수 있음을 설정
	virtual void EndFocus();
	virtual void BeginInteract(); // e를 길게 눌러 열면 시작상호작용을 사용하여 1~2초 정도 기다리게
	virtual void EndInteract();
	virtual void Interact(APixelCodeCharacter* PlayerCharacter); // 플레이어 캐릭터를 위한 상호작용인수 제공

	FInteractableData InteractableData;//  상호작용 가능 데이터라는 구조체

};
