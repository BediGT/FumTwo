// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumEquipmentType.generated.h"

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	Empty = 0 UMETA(DisplayName = "Empty"),
	BubbleShield = 1 UMETA(DisplayName = "Bubble Shield"),
	BouncePad = 2 UMETA(DisplayName = "Bounce Pad"),
};
