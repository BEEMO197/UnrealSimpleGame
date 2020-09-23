// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	UStaticMesh* Asset = MeshAsset.Object;
	TileMesh->SetStaticMesh(Asset);

	ConstructorHelpers::FObjectFinder<UMaterial>TileMaterial(TEXT("Material'/Game/Assets/TileColour.TileColour'"));
	UMaterial* Material = TileMaterial.Object;

	AvailableMaterials.Add(Material);

	ConstructorHelpers::FObjectFinder<UMaterial>TileMaterial1(TEXT("Material'/Game/Assets/Obstacle.Obstacle'"));
	Material = TileMaterial1.Object;

	AvailableMaterials.Add(Material);

	ConstructorHelpers::FObjectFinder<UMaterial>TileMaterial2(TEXT("Material'/Game/Assets/Hazzard.Hazzard'"));
	Material = TileMaterial2.Object;

	AvailableMaterials.Add(Material);

	CurrentMaterial = AvailableMaterials[0];
	TileMesh->SetMaterial(0, CurrentMaterial);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Obstacle)
		CurrentMaterial = AvailableMaterials[1];

	else if (Hazzard)
		CurrentMaterial = AvailableMaterials[2];
	TileMesh->SetMaterial(0, CurrentMaterial);
}

