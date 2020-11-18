// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = 30;
	maxHealth = health;

	healthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	healthBar->SetupAttachment(GetCapsuleComponent());
	healthBar->SetRelativeLocation(FVector(-5.0f, 0.0f, 100.0f));
	healthBar->SetDrawSize(FVector2D(100.0f, 10.0f));

	// Setup Sphere Collision for Enemies
	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCol->SetupAttachment(RootComponent);
	SphereCol->SetSphereRadius(300.f);
	SphereCol->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemy::OnSphereOverlapBegin);
	SphereCol->OnComponentEndOverlap.AddDynamic(this, &AMyEnemy::OnSphereOverlapEnd);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemy::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyEnemy::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();

	FRandomStream FStream;
	FStream.GenerateNewSeed();
	float randXPosNeg = FStream.RandRange(1, 2);
	float randYPosNeg = FStream.RandRange(1, 2);
	float randX = FStream.RandRange(0, 5) * 200 * (randXPosNeg == 1 ? -1 : 1);
	float randY = FStream.RandRange(0, 5) * 200 * (randYPosNeg == 1 ? -1 : 1);

	if (randX > 800)
	{
		randX = 800;
	}

	if (randY > 800)
	{
		randY = 800;
	}
	SetActorLocation(FVector(randX, randY, GetActorLocation().Z + 500));
}

// Called every frame
void AMyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAttacking)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetEnemyLocation(), FVector(characterActor->GetActorLocation().X, characterActor->GetActorLocation().Y, GetEnemyLocation().Z)));
	}
}

UWidgetComponent* AMyEnemy::getHealthBarWidge()
{
	return healthBar;
}

FVector AMyEnemy::GetEnemyLocation()
{
	return GetActorLocation();
}

bool AMyEnemy::GetIsAttacking()
{
	return IsAttacking;
}

bool AMyEnemy::GetAnimationAttack()
{
	return AnimationAttack;
}

void AMyEnemy::SetAnimationAttack(bool animAttack)
{
	AnimationAttack = animAttack;
}

void AMyEnemy::Damage(int32 damage)
{
	FString DamagePrnt = "Health: ";
	DamagePrnt.AppendInt(health);
	DamagePrnt.Append(" / ");
	DamagePrnt.AppendInt(maxHealth);


	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Turquoise, DamagePrnt);

	health -= damage;
}

int32 AMyEnemy::GetHealth()
{
	return health;
}

int32 AMyEnemy::GetMaxHealth()
{
	return maxHealth;
}

bool AMyEnemy::GetIsDying()
{
	return isDying;
}

void AMyEnemy::SetIsDying(bool Dying)
{
	isDying = Dying;
	FTimerHandle deathChange;
	GetWorld()->GetTimerManager().SetTimer(deathChange, this, &AMyEnemy::DestroyDeadEnemy, 3.0f, false);
}

void AMyEnemy::DestroyDeadEnemy()
{
	Destroy();
}

void AMyEnemy::SetIsAttacking()
{
	IsAttacking = !IsAttacking;
}

ATile* AMyEnemy::GetTile()
{
	return tile;
}

TArray<ATile*> AMyEnemy::GetWalkableTiles()
{
	return walkableTiles;
}

void AMyEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Turquoise, FString("OnBeginOverlap() called With: ") + OtherActor->GetName());

	if (OtherActor->IsA<ATile>())
	{
		tile = (ATile*)OtherActor;
		if (tile->CurrentTileType == TileType::Obstacle)
		{
			FRandomStream FStream;
			FStream.GenerateNewSeed();
			float randXPosNeg = FStream.RandRange(1, 2);
			float randYPosNeg = FStream.RandRange(1, 2);
			float randX = FStream.RandRange(0, 5) * 200 * (randXPosNeg == 1 ? -1 : 1);
			float randY = FStream.RandRange(0, 5) * 200 * (randYPosNeg == 1 ? -1 : 1);

			if (randX > 800)
			{
				randX = 800;
			}

			if (randY > 800)
			{
				randY = 800;
			}
			SetActorLocation(FVector(randX, randY, GetActorLocation().Z + 500));
		}
	}
	if (IsValid(tile))
	{
		walkableTiles = tile->GetAdjacentTiles();
	}
}

void AMyEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, FString("OnEndOverlap() called With: ") + OtherActor->GetName());

	if (OtherActor->IsA<ATile>())
	{
		if (tile != nullptr)
		{
			walkableTiles.Empty();
			tile->tileDirectionsFromPlayer.Empty();
			tile->foundATiles = false;
			tile = nullptr;
		}
	}
}

void AMyEnemy::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->IsA<AMyCharacter>())
	{
		if (OtherComp->IsA<USkeletalMeshComponent>())
		{
			characterActor = OtherActor;
			IsAttacking = true;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, FString("OnSphereBeginOverlap() called With: ") + OtherActor->GetName());
	}
}

void AMyEnemy::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor->IsA<AMyCharacter>())
	{
		if (OtherComp->IsA<USkeletalMeshComponent>())
		{
			IsAttacking = false;
		}
	}
}
