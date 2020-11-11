// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

UCLASS()
class DESN_PEREIRA_MATTHEW_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	AActor* characterActor;

	UPROPERTY(EditAnywhere)
		FTimerHandle attackingTimer;

	UPROPERTY(EditAnywhere)
		bool AnimationAttack = false;

	UPROPERTY(EditAnywhere)
		bool IsAttacking = false;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetAnimationAttack();

	UFUNCTION(BlueprintCallable)
		void SetAnimationAttack(bool animAttack);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetIsAttacking();

	UFUNCTION(BlueprintCallable)
		void SetIsAttacking();

	UPROPERTY(EditAnywhere)
		ATile* tile;

	UPROPERTY(EditAnywhere)
		TArray<ATile*> walkableTiles;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		ATile* GetTile();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		TArray<ATile*> GetWalkableTiles();

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FVector GetEnemyLocation();

};
