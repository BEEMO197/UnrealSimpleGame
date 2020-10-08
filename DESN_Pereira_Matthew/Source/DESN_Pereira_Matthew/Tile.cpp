// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "MyCharacter.h"

// Sets default values
ATile::ATile()
{
	FRandomStream FStream;
	FStream.GenerateNewSeed();
	randomNumber = FStream.RandRange(1, 10);

	if (randomNumber == 2)
	{
		CurrentTileType = Hazzard;
	}

	else if (randomNumber == 7)
	{
		CurrentTileType = Obstacle;
	}

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->SetWorldScale3D(FVector(2.0f));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(TileMesh);
	BoxComponent->SetBoxExtent(FVector(20.f, 20.f, 10.f));

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, 110.f));

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATile::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATile::OnOverlapEnd);

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	UStaticMesh* Asset = MeshAsset.Object;
	TileMesh->SetStaticMesh(Asset);

	ConstructorHelpers::FObjectFinder<UMaterial>TileMaterial(TEXT("Material'/Game/Assets/Materials/TileColour.TileColour'"));
	UMaterial* Material = TileMaterial.Object;

	AvailableMaterials.Add(Material);

	ConstructorHelpers::FObjectFinder<UMaterial>TileMaterial1(TEXT("Material'/Game/Assets/Materials/Obstacle.Obstacle'"));
	Material = TileMaterial1.Object;

	AvailableMaterials.Add(Material);

	ConstructorHelpers::FObjectFinder<UMaterial>TileMaterial2(TEXT("Material'/Game/Assets/Materials/Hazzard.Hazzard'"));
	Material = TileMaterial2.Object;

	AvailableMaterials.Add(Material);

	ConstructorHelpers::FObjectFinder<UMaterial>TileMaterial3(TEXT("Material'/Game/Assets/Materials/Moveable.Moveable'"));
	Material = TileMaterial3.Object;

	AvailableMaterials.Add(Material);

	CurrentMaterial = AvailableMaterials[0];
	TileMesh->SetMaterial(0, CurrentMaterial);
}


// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

	init();
	foundATiles = false;
}

void ATile::init()
{
	// Check if Obstacle or Hazzard
	if (CurrentTileType == Obstacle)
	{
		// Set Obstacle Stuff
		FVector loc = GetActorLocation();
		loc.Z += 100.f;
		SetActorLocation(loc);
	}

	else if (CurrentTileType == Hazzard)
	{
		// Set Hazzard Stuff
	}
}

void ATile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AEnemy>())
	{
		enemyOnTile = Cast<AEnemy>(OtherActor);
		
		enemyOnTile->EnemyMesh->OnClicked.AddDynamic(Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)), &AMyCharacter::OnClicked);
	}
}

void ATile::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AEnemy>())
	{
		enemyOnTile->EnemyMesh->OnClicked.RemoveDynamic(Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)), &AMyCharacter::OnClicked);
		enemyOnTile = NULL;
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult Outhit;

	if (!foundATiles)
	{
		// Get Front Tile
		FVector Start = TileMesh->GetComponentLocation();
		Start.Z += 10.f;

		FVector End = Start;
		End.X += 101.f;

		FCollisionQueryParams CollisionParams;

		bool isHit = GetWorld()->LineTraceSingleByChannel(Outhit, End, Start, ECC_Visibility, CollisionParams);

		if (isHit)
		{
			if (Outhit.bBlockingHit && Outhit.GetActor() != this)
			{
				foundATiles = true;
				AdjacentTiles.Add((ATile*)Outhit.GetActor());
			}
		}

		// Get Back Tile

		End = Start;
		End.X -= 101.f;

		isHit = GetWorld()->LineTraceSingleByChannel(Outhit, End, Start, ECC_Visibility, CollisionParams);

		if (isHit)
		{
			if (Outhit.bBlockingHit && Outhit.GetActor() != this)
			{
				foundATiles = true;
				AdjacentTiles.Add((ATile*)Outhit.GetActor());
			}
		}

		// Get Left Tile
		End = Start;
		End.Y -= 101.f;

		isHit = GetWorld()->LineTraceSingleByChannel(Outhit, End, Start, ECC_Visibility, CollisionParams);

		if (isHit)
		{
			if (Outhit.bBlockingHit && Outhit.GetActor() != this)
			{
				foundATiles = true;
				AdjacentTiles.Add((ATile*)Outhit.GetActor());
			}
		}

		// Get Right Tile
		End = Start;
		End.Y += 101.f;

		isHit = GetWorld()->LineTraceSingleByChannel(Outhit, End, Start, ECC_Visibility, CollisionParams);

		if (isHit)
		{
			if (Outhit.bBlockingHit && Outhit.GetActor() != this)
			{
				foundATiles = true;
				AdjacentTiles.Add((ATile*)Outhit.GetActor());
			}
		}
	}
	
	switch (CurrentTileType)
	{
	case Default:
		CurrentMaterial = AvailableMaterials[0];
		break;

	case Obstacle:
		CurrentMaterial = AvailableMaterials[1];
		break;

	case Hazzard:
		CurrentMaterial = AvailableMaterials[2];
		break;

	case Selected:
		CurrentMaterial = AvailableMaterials[3];
		break;

	default:
		CurrentMaterial = AvailableMaterials[0];
		break;
	}

	TileMesh->SetMaterial(0, CurrentMaterial);
}

