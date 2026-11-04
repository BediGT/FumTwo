// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameSettings.generated.h"

USTRUCT()
struct FUMTWO_API FGameSettings
{
	GENERATED_BODY()
	
	UPROPERTY()
	float Sensitivity = 1.f;
	
	UPROPERTY()
	float FieldOfView = 1.f;
};
