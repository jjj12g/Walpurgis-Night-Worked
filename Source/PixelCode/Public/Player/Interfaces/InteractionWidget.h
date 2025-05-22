// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class APixelCodeCharacter;
struct FInteractableData; // 컴파일러가 보고 다른곳에서 정의될 유형임을 알 수 있도록 이 파일 클래스 외부 어딘가에 배치하는 것과 동일한 방식으로 대부분의 항목전달
class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class PIXELCODE_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	APixelCodeCharacter* PlayerReference;

	void UpdateWidget(const FInteractableData& InteractableData)const; // 모든 다른 텍스트블록 업데이트

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* NameText; // 이름

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* ActionText; // 작업

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* QuantityText; // 수량

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* KeyPressText; // 키누르기 텍스트

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UProgressBar* InteractionProgressBar; // 시간표시줄

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	float CurrentInteractionDuration;

	UFUNCTION(Category = "Interaction Widget | Interactable Data")
	float UpdateInteractionProgress();// 소수점 반환함수, 상호작용 진행률 업데이트

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

};

