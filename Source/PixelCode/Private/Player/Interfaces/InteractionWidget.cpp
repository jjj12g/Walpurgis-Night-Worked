// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interfaces/InteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Player/Interfaces/InteractionInterface.h"

void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress"); // 퍼센트 업데이트

}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f; //현재 상호작용 시간
}

void UInteractionWidget::UpdateWidget(const FInteractableData& InteractableData) const
{
	switch (InteractableData.InteractableType)
	{
	case EInteractableType::Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if (InteractableData.Quantity < 2) // 수량이 1이면 안보이도록
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityText->SetText(FText::Format(FText::FromString("x{0}"), InteractableData.Quantity)); // 인수 0을가져와 여기에 알고있는 다른숫자가 있으면 거기에넣음.
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;

	case EInteractableType::NonPlayerCharacter:
		break;

	case EInteractableType::Device:
		//InteractionProgressBar->SetVisibility(ESlateVisibility::Visible);
		break;

	case EInteractableType::Toggle:
		break;

	case EInteractableType::Container:
		break;

	default:
		break;
	}

	ActionText->SetText(InteractableData.Action);
	NameText->SetText(InteractableData.Name);
}

float UInteractionWidget::UpdateInteractionProgress()
{


	return 0.0f;
}