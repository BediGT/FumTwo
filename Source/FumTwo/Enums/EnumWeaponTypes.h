// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unarmed = 0    UMETA(DisplayName = "Unarmed"),
	Pistol = 1     UMETA(DisplayName = "Pistol"),
	AssaultRifle = 2 UMETA(DisplayName = "Assault Rifle"),
	SniperRifle = 3 UMETA(DisplayName = "Sniper Rifle"),
	Shotgun = 4 UMETA(DisplayName = "Shotgun")
};
