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

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(TileMesh);
	BoxComponent->SetBoxExtent(FVector(5.f, 5.f, 10.f));

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, 110.f));

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

	ConstructorHelpers::FObjectFinder<UMaterial>TileMaterial3(TEXT("Material'/Game/Assets/Moveable.Moveable'"));
	Material = TileMaterial3.Object;

	AvailableMaterials.Add(Material);

	CurrentMaterial = AvailableMaterials[0];
	TileMesh->SetMaterial(0, CurrentMaterial);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	FoundTiles[0] = false;
	FoundTiles[1] = false;
	FoundTiles[2] = false;
	FoundTiles[3] = false;
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult Outhit;

	FVector Start = TileMesh->GetComponentLocation();
	Start.Z += 10.f;

	FVector End = Start;
	End.X += 101.f;

	FCollisionQueryParams CollisionParams;

	bool isHit = GetWorld()->LineTraceSingleByChannel(Outhit, End, Start, ECC_Visibility, CollisionParams);

	int count = 0;
	if (isHit)
	{
		if (Outhit.bBlockingHit && Outhit.GetActor() != this)
		{
			FoundTiles[count++] = true;
			AdjacentTiles.Add((ATile*)Outhit.GetActor());
		}
	}

	End = Start;
	End.X -= 101.f;

	isHit = GetWorld()->LineTraceSingleByChannel(Outhit, End, Start, ECC_Visibility, CollisionParams);

	if (isHit)
	{
		if (Outhit.bBlockingHit && Outhit.GetActor() != this)
		{
			FoundTiles[count++] = true;
			AdjacentTiles.Add((ATile*)Outhit.GetActor());
		}
	}

	End = Start;
	End.Y -= 101.f;

	 isHit = GetWorld()->LineTraceSingleByChannel(Outhit, End, Start, ECC_Visibility, CollisionParams);

	if (isHit)
	{
		if (Outhit.bBlockingHit && Outhit.GetActor() != this)
		{
			FoundTiles[count++] = true;
			AdjacentTiles.Add((ATile*)Outhit.GetActor());
		}
	}

	End = Start;
	End.Y += 101.f;

	isHit = GetWorld()->LineTraceSingleByChannel(Outhit, End, Start, ECC_Visibility, CollisionParams);

	if (isHit)
	{
		if (Outhit.bBlockingHit && Outhit.GetActor() != this)
		{
			FoundTiles[count++] = true;
			AdjacentTiles.Add((ATile*)Outhit.GetActor());
		}
	}


	if (selected)
		CurrentMaterial = AvailableMaterials[3];

	else if (Hazzard)
		CurrentMaterial = AvailableMaterials[2];

	else if (Obstacle)
		CurrentMaterial = AvailableMaterials[1];

	else
		CurrentMaterial = AvailableMaterials[0];

	TileMesh->SetMaterial(0, CurrentMaterial);
}

