// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create our components
	RootComponent = GetCapsuleComponent();

	// Set Character Size
	//RootComponent->SetWorldScale3D(FVector(0.75f));

	// Setup CameraSpringArm
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-45.0f, 0.0f, 0.0f));
	CameraSpringArm->TargetArmLength = 400.f;
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 3.0f;

	// Setup Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	// Setup Sphere Collision for Enemies
	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCol->SetupAttachment(RootComponent);
	SphereCol->SetSphereRadius(300.f);
	SphereCol->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnSphereOverlapBegin);
	SphereCol->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnSphereOverlapEnd);

	// Setup Capsule Collision for movement
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyCharacter::OnHit);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnOverlapEnd);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-2, 0, FColor::MakeRandomColor(), GetActorLocation().ToString());
	if (enemies.Num() > 0)
	{
		//GEngine->AddOnScreenDebugMessage(5, 0, FColor::Green, "OnClicked() called to " + enemies.Last()->GetName());
	}
}

bool AMyCharacter::getAttacking()
{
	return attacking;
}

FVector AMyCharacter::getCharacterLocation()
{
	return GetActorLocation();
}

void AMyCharacter::ChangeAttack()
{
	attacking = false;
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// Set up "Camera" bindings.
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::AddControllerYawCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::AddControllerPitchCamera);

	PlayerInputComponent->BindAxis("Zoom", this, &AMyCharacter::ZoomCamera);

	
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();

	MyController->bShowMouseCursor = true;
	MyController->bEnableClickEvents = true;
	MyController->bEnableMouseOverEvents = true;

}

void AMyCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
	
}

void AMyCharacter::AddControllerPitchCamera(float Value)
{
	FRotator rotate = CameraSpringArm->GetRelativeRotation();
	rotate.Pitch += Value * 2.5f;
	CameraSpringArm->SetRelativeRotation(rotate);
}

void AMyCharacter::AddControllerYawCamera(float Value)
{
	FRotator rotate = CameraSpringArm->GetRelativeRotation();
	rotate.Yaw += Value * 2.5f;
	CameraSpringArm->SetRelativeRotation(rotate);
}

void AMyCharacter::ZoomCamera(float Value)
{
	float zoom = CameraSpringArm->TargetArmLength;
	zoom -= Value * 50.f;

	if (zoom > 1500.f)
	{
		zoom = 1500.f;
	}

	else if (zoom < 500.f)
	{
		zoom = 500.f;
	}

	CameraSpringArm->TargetArmLength = zoom;
}

void AMyCharacter::OnClicked(UPrimitiveComponent* pComponent, FKey inKey)
{
	int32 count = 0;

	FVector moveToLocation;
	FVector rotationVector;
	FRotator rotation;

	FTimerHandle attackChange;

	// Click on enemy
	//for (AMyEnemy* enemy : enemies)
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Cyan, "OnClicked() called to " + pComponent->GetOwner()->GetName());
	//	if (enemy == pComponent->GetOwner())
	//	{
	//		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, "OnClicked() called to " + pComponent->GetOwner()->GetClass()->GetDisplayNameText().ToString());
	//
	//		rotationVector = FVector(enemy->GetEnemyLocation() - GetActorLocation());
	//		rotationVector.Normalize();
	//		rotation = rotationVector.Rotation();
	//		rotation.Roll = 0.f;
	//		rotation.Pitch = 0.f;
	//		rotation.Yaw -= 90.f;
	//		GetMesh()->SetWorldRotation(rotation);
	//
	//		attacking = true;
	//		GetWorld()->GetTimerManager().SetTimer(attackChange, this, &AMyCharacter::ChangeAttack, 1.0f, false);
	//		continue;
	//	}
	//}

	if (pComponent->GetOwner()->IsA<AMyEnemy>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::MakeRandomColor(), "OnClicked() called to " + pComponent->GetOwner()->GetName());

		rotationVector = FVector(Cast<AMyEnemy>(pComponent->GetOwner())->GetEnemyLocation() - GetActorLocation());
		rotationVector.Normalize();
		rotation = rotationVector.Rotation();
		rotation.Roll = 0.f;
		rotation.Pitch = 0.f;
		rotation.Yaw -= 90.f;
		GetMesh()->SetWorldRotation(rotation);

		attacking = true;
		GetWorld()->GetTimerManager().SetTimer(attackChange, this, &AMyCharacter::ChangeAttack, 1.0f, false);
	}

	for (auto adjTile : walkableTiles)
	{
		if (adjTile == pComponent->GetOwner())
		{
			switch (count)
			{
				// Front Tile
			case 0:
				moveToLocation = adjTile->BoxComponent->GetComponentLocation();
				//moveToLocation.X += 75.f;
				//moveToLocation.Z += 88.f;
				break;

				// Back Tile
			case 1:
				moveToLocation = adjTile->BoxComponent->GetComponentLocation();
				//moveToLocation.X -= 75.f;
				//moveToLocation.Z += 88.f;
				break;

				// Left Tile
			case 2:
				moveToLocation = adjTile->BoxComponent->GetComponentLocation();
				//moveToLocation.Y -= 75.f;
				//moveToLocation.Z += 88.f;
				break;

				// Right Tile
			case 3:
				moveToLocation = adjTile->BoxComponent->GetComponentLocation();
				//moveToLocation.Y += 75.f;
				//moveToLocation.Z += 88.f;
				break;
				
			default:
				break;
			}

			rotationVector = FVector(moveToLocation - GetActorLocation());
			rotationVector.Normalize();
			rotation = rotationVector.Rotation();
			rotation.Roll = 0.f;
			rotation.Pitch = 0.f;
			rotation.Yaw -= 90.f;
			GetMesh()->SetWorldRotation(rotation);

			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), moveToLocation);

			continue;
		}
		count++;
	}	
}

void AMyCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, "OnHit() called");
}

void AMyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, FString("OnBeginOverlap() called With: ") + OtherActor->GetName());

	if (OtherActor->IsA<ATile>())
	{
		tile = (ATile*)OtherActor;
	}

	if (IsValid(tile))
	{
		walkableTiles = tile->GetAdjacentTiles();
		for (auto adjTile : walkableTiles)
		{
			adjTile->PreviousTileType = adjTile->CurrentTileType;
			adjTile->CurrentTileType = TileType::Selected;
			adjTile->TileMesh->OnClicked.AddDynamic(this, &AMyCharacter::OnClicked);
		}
	}
}

void AMyCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, FString("OnEndOverlap() called With: ") + OtherActor->GetName());

	if (tile != nullptr)
	{

		for (auto adjTile : walkableTiles)
		{
			adjTile->CurrentTileType = adjTile->PreviousTileType;
			adjTile->TileMesh->OnClicked.RemoveDynamic(this, &AMyCharacter::OnClicked);
		}

		walkableTiles.Empty();
		tile->foundATiles = false;
		tile = nullptr;
	}
}

void AMyCharacter::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100, FColor::MakeRandomColor(), FString("OnSphereBeginOverlap() called With: ") + OtherComp->GetName());

	FTimerHandle attackChange;

	if (OtherActor->IsA<AMyEnemy>())
	{
		if (OtherComp->IsA<USkeletalMeshComponent>())
		{
			//Cast<AMyEnemy>(OtherActor)->GetMesh()->OnClicked.AddDynamic(this, &AMyCharacter::OnClicked);
			enemies.Add(Cast<AMyEnemy>(OtherActor));
			enemies.Last()->GetMesh()->OnClicked.AddDynamic(this, &AMyCharacter::OnClicked);
			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 100, FColor::MakeRandomColor(), FString("OnSphereBeginOverlap() called With: ") + OtherComp->GetName());
			//OtherComp->OnClicked.AddDynamic(this, &AMyCharacter::OnClicked);
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, FString("OnSphereBeginOverlap() called With: ") + OtherActor->GetName());
	}
}

void AMyCharacter::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, FString("OnSphereEndOverlap() called With: ") + OtherActor->GetName());
	for(AMyEnemy* enemy : enemies)
	{
		if (enemy == OtherActor)
		{
			enemy->GetMesh()->OnClicked.RemoveDynamic(this, &AMyCharacter::OnClicked);
			enemies.Remove(enemy);
		}
	}
}
