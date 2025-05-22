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

	InteractableData = InstanceInteractableData; // �ν��Ͻ� �����Ϳ� ���Լ���

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
		Mesh->SetRenderCustomDepth(true); // ���Ǳ��� ������ �ο� �Է°� ����

	}
}

void AInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false); // ���Ǳ��� ������ �ο� �Է°� ����
	}
}


