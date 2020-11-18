// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM()
enum WeaponType
{
	Melee = 1 UMETA(DisplayName = "Melee"),
	Ranged = 2 UMETA(DisplayName = "Ranged"),
	RangedAOE = 3 UMETA(DisplayName = "RangedAOE"),
};

/**
 * 
 */
class DESN_PEREIRA_MATTHEW_API Weapon
{
public:
	Weapon();
	~Weapon();

	UPROPERTY(EditAnywhere)
		WeaponType currentWeapon;

	UPROPERTY(EditAnywhere)
		int32 weaponDamage;
};
