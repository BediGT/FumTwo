// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RealMaterialDataAsset.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FUMTWO_API URealMaterialDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Material Parameters")
	double YieldStrength = 1.f;

	UPROPERTY(EditAnywhere, Category = "Material Parameters")
	double YoungModulus = 1.f;

	UPROPERTY(EditAnywhere, Category = "Material Parameters")
	double Density = 1.f;
};
