// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../DataAssets/RealMaterialDataAsset.h"
#include "RealTargetInterface.generated.h"

UINTERFACE()
class URealTargetInterface : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IRealTargetInterface
{
	GENERATED_BODY()

public:
	virtual const URealMaterialDataAsset* GetMaterial() const = 0;
	virtual const void OnImpact(const FVector& ImpactPoint, const FVector& ImpactDirection, double PenetrationDepth, double Radius) = 0;
};
