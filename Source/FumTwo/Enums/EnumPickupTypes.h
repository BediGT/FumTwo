// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumPickupTypes.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Weapon = 0 UMETA(DisplayName = "Weapon"),
	Grenade = 1 UMETA(DisplayName = "Grenade"),
	Equipment = 2 UMETA(DisplayName = "Equipment"),
	Health = 3 UMETA(DisplayName = "Health"),
};
