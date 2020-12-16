// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"
#include "MyCharacter.generated.h"


UCLASS()
class DESN_PEREIRA_MATTHEW_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~       COMPONENTS      ~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	UPROPERTY(EditAnywhere)
		USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCol;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~      VARIABLES       ~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	UPROPERTY(EditAnywhere)
		UParticleSystem* RangedAOEAttack;

	UPROPERTY(EditAnywhere)
		USoundBase* rangedAOEAudio;

	UPROPERTY(EditAnywhere)
		int enemyCount;

	Weapon weapon;

	UPROPERTY(EditAnywhere)
		int32 health;

	UPROPERTY(EditAnywhere)
		int32 maxHealth;

	UPROPERTY(EditAnywhere)
		UWidgetComponent* healthBar;

	// Current Tile standing on
	UPROPERTY(EditAnywhere)
		ATile* tile;

	UPROPERTY(EditAnywhere)
		TArray<ATile*> walkableTiles;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> enemies;

	UPROPERTY(EditAnywhere)
		bool canAttack = true;

	UPROPERTY(EditAnywhere)
		bool attacking = false;

	bool goodSpawn = false;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~      GAME FUNCTIONS       ~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FTimerHandle HazzardDamageTimer;
	FTimerHandle AttackTimer;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		UWidgetComponent* getHealthBarWidge();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetHealth();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetMaxHealth();

	UFUNCTION(BlueprintCallable)
		void Damage(int32 damage);

	UFUNCTION(BlueprintCallable)
		void HazzardDamage();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool getAttacking();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector getCharacterLocation();

	UFUNCTION()
		void ChangeAttack(AMyEnemy* enemy);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		WeaponType GetCurrentWeapon();

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~      MOVEMENT / CAMERA       ~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles input for moving forward and backward.
	UFUNCTION()
		void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
		void MoveRight(float Value);

	// Handles Input for camera
	UFUNCTION()
		void AddControllerPitchCamera(float Value);

	UFUNCTION()
		void AddControllerYawCamera(float Value);

	// Handles Camera Zoom
	UFUNCTION()
		void ZoomCamera(float Value);

	UFUNCTION()
		void ChangeWeapons();

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~       EVENTS / DELEGATES       ~~~~~~~~~~~~~~~~~~~*/
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	// Gets Mouse Input for Movement
	UFUNCTION()
		void OnClicked(UPrimitiveComponent* pComponent, FKey inKey);

	// Collison Functions

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
