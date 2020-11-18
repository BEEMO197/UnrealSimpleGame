// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "RangedAOEAttack.h"

// Sets default values
ARangedAOEAttack::ARangedAOEAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCol->SetupAttachment(RootComponent);
	SphereCol->SetSphereRadius(300.0f);

}

// Called when the game starts or when spawned
void ARangedAOEAttack::BeginPlay()
{
	Super::BeginPlay();
	
	rangedAOEDamage.currentWeapon = WeaponType::RangedAOE;

	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),)
}

// Called every frame
void ARangedAOEAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARangedAOEAttack::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->IsA<AMyEnemy>())
	{
		if (OtherComp->IsA<USkeletalMeshComponent>())
		{
			Cast<AMyEnemy>(OtherActor)->Damage(rangedAOEDamage.weaponDamage);

			if (Cast<AMyEnemy>(OtherActor)->health <= 0)
			{
				Cast<AMyEnemy>(OtherActor)->SetIsDying(true);
			}
		}
	}
}

void ARangedAOEAttack::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor->IsA<AMyEnemy>())
	{
		if (OtherComp->IsA<USkeletalMeshComponent>())
		{
		}
	}
}
