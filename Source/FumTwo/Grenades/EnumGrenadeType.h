// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGrenadeType : uint8
{
	Frag = 0    UMETA(DisplayName = "Frag"),
	Plasma = 1     UMETA(DisplayName = "Plasma"),
};

