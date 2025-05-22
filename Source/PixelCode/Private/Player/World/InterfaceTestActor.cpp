// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/World/InterfaceTestActor.h"
#include "Components/StaticMeshComponent.h"
//#include "portal/portalActor.h"
#include "Player/Interfaces/InteractionWidget.h"
//#include "portal/portalBarWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>


// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData; // 인스턴스 데이터와 같게설정

	InteractionWidget = Cast<UInteractionWidget>(InteractionWidget);
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true); // 정의깊이 랜더링 부울 입력값 설정

	}
}

void AInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false); // 정의깊이 랜더링 부울 입력값 설정
	}
}


