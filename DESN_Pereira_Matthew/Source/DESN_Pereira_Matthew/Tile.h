// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class DESN_PEREIRA_MATTHEW_API ATile : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TileMesh;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
