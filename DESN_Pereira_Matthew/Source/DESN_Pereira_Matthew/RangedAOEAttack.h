// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Weapon.h"
#include "RangedAOEAttack.generated.h"

UCLASS()
class DESN_PEREIRA_MATTHEW_API ARangedAOEAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangedAOEAttack();

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCol;

	Weapon rangedAOEDamage;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void Explode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
