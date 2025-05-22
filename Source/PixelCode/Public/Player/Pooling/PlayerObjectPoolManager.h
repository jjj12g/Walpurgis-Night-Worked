// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerObjectPoolManager.generated.h"

class APixelCodeCharacter;

UCLASS()
class PIXELCODE_API APlayerObjectPoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerObjectPoolManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pool")
	TSubclassOf<APixelCodeCharacter> CharacterClass; // 풀에 사용할 캐릭터 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	TArray<APixelCodeCharacter*> Pool; // 캐릭터 풀


	APixelCodeCharacter* GetPooledCharacter(); // 캐릭터 가져오기
	void ReturnPooledCharacter(APixelCodeCharacter* PlayerChar); // 캐릭터 반환하기


private:
	UPROPERTY(EditAnywhere, Category = "Pool")
	int32 MaxPoolSize; // 풀의 최대 크기

};
