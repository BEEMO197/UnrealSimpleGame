// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Tile.generated.h"



UENUM()
enum TileType
{
	Default = 0 UMETA(DisplayName = "Default"),
	Obstacle = 1 UMETA(DisplayName = "Obstacle"),
	Hazzard = 2 UMETA(DisplayName = "Hazzard"),
	Selected = 3 UMETA(DisplayName = "Selected"),
};

UENUM()
enum TileDirectionFromPlayer
{
	CurrentTile = 0 UMETA(DisplayName = "Default"),
	xPos = 1 UMETA(DisplayName = "X Positive"),
	xNeg = 2 UMETA(DisplayName = "X Negative"),
	yPos = 3 UMETA(DisplayName = "Y Positive"),
	yNeg = 4 UMETA(DisplayName = "Y Negative"),
};

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

	UFUNCTION()
		void init();
public:	

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~       COMPONENTS      ~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~      VARIABLES       ~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	UPROPERTY(VisibleAnywhere)
		TArray<ATile*> AdjacentTiles;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<TileType> CurrentTileType;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<TileType> PreviousTileType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<TileDirectionFromPlayer> tileDirectionFromPlayer;

	int32 i = 0;
	int32 randomNumber;

	bool foundATiles;

	TArray<UMaterial*> AvailableMaterials;

	UMaterial* CurrentMaterial;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~      VARIABLE FUNCTIONS       ~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TArray<ATile*> GetAdjacentTiles();

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~       EVENTS / DELEGATES       ~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~      GAME FUNCTIONS       ~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};