// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyEnemy::AMyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemy::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyEnemy::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, FString("OnBeginOverlap() called With: ") + OtherActor->GetName());

	if (OtherActor->IsA<ATile>())
	{
		tile = (ATile*)OtherActor;
	}
	else if (OtherActor->IsA<AMyCharacter>())
	{
		//FTimerHandle attackingTimer;
		//GetWorld()->GetTimerManager().SetTimer(attackingTimer, this, &AMyEnemy::SetIsAttacking, 1.0f, true);
		characterActor = OtherActor;
		IsAttacking = true;
	}

	if (IsValid(tile))
	{
		walkableTiles = tile->GetAdjacentTiles();
		for (auto adjTile : walkableTiles)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, adjTile->GetName());
			switch (adjTile->tileDirectionFromPlayer)
			{

			case CurrentTile:
				GEngine->AddOnScreenDebugMessage(1, 5, FColor::MakeRandomColor(), "Current Tile");
				break;

			case xPos:
				GEngine->AddOnScreenDebugMessage(2, 5, FColor::MakeRandomColor(), "x Positive");
				break;

			case xNeg:
				GEngine->AddOnScreenDebugMessage(3, 5, FColor::MakeRandomColor(), "x Negative");
				break;

			case yPos:
				GEngine->AddOnScreenDebugMessage(4, 5, FColor::MakeRandomColor(), "y Positive");
				break;

			case yNeg:
				GEngine->AddOnScreenDebugMessage(5, 5, FColor::MakeRandomColor(), "y Negative");
				break;

			default:
				break;
			}
		}
	}
}

void AMyEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, FString("OnEndOverlap() called With: ") + OtherActor->GetName());

	if (tile != nullptr)
	{
		walkableTiles.Empty();
		tile->foundATiles = false;
		tile = nullptr;
	}

	if (OtherActor->IsA<AMyCharacter>())
	{
		IsAttacking = false;
	}
}