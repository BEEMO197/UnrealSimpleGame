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

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-45.0f, 0.0f, 0.0f));
	CameraSpringArm->TargetArmLength = 400.f;
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 3.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

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
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, "OnClicked() called to " +	pComponent->GetOwner()->GetName());
	int32 count = 0;

	FVector moveToLocation;
	FVector rotationVector;
	FRotator rotation;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, "OnClicked() called to " + pComponent->GetOwner()->GetClass()->GetDisplayNameText().ToString());

	for (auto adjTile : tile->AdjacentTiles)
	{
		if (adjTile == pComponent->GetOwner() || adjTile->enemyOnTile == pComponent->GetOwner())
		{
			switch (count)
			{
				// Front Tile
			case 0:
				moveToLocation = adjTile->BoxComponent->GetComponentLocation();
				//moveToLocation.X += 75.f;
				moveToLocation.Z += 88.f;
				break;

				// Back Tile
			case 1:
				moveToLocation = adjTile->BoxComponent->GetComponentLocation();
				//moveToLocation.X -= 75.f;
				moveToLocation.Z += 88.f;
				break;

				// Left Tile
			case 2:
				moveToLocation = adjTile->BoxComponent->GetComponentLocation();
				//moveToLocation.Y -= 75.f;
				moveToLocation.Z += 88.f;
				break;

				// Right Tile
			case 3:
				moveToLocation = adjTile->BoxComponent->GetComponentLocation();
				//moveToLocation.Y += 75.f;
				moveToLocation.Z += 88.f;
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
			
			if (adjTile->enemyOnTile != NULL || adjTile->enemyOnTile == pComponent->GetOwner())
			{
				attacking = true;
				GetWorldTimerManager().SetTimer(AttackTimer, this, &AMyCharacter::ChangeAttack, 1.f, false);
				continue;
			}

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
	tile = (ATile*)OtherActor;

	if (IsValid(tile))
	{
		for (auto adjTile : tile->AdjacentTiles)
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
		for (auto adjTile : tile->AdjacentTiles)
		{
			adjTile->CurrentTileType = adjTile->PreviousTileType;
			adjTile->TileMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

			adjTile->TileMesh->OnClicked.RemoveDynamic(this, &AMyCharacter::OnClicked);
			
		}
		tile = nullptr;
	}
}
