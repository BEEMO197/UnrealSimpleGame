// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "TileMap.generated.h"

UCLASS()
class DESN_PEREIRA_MATTHEW_API ATileMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere)
		FVector CurrentTileLocation;

	UPROPERTY(EditAnywhere)
		float StartX;

	UPROPERTY(EditAnywhere)
		float StartY;

	UPROPERTY(EditAnywhere)
		float StartZ;

	UPROPERTY(EditAnywhere)
		int32 Row = 10;

	UPROPERTY(EditAnywhere)
		int32 Column = 10;

	UPROPERTY(EditAnywhere)
		TArray<ATile*> Grid;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
