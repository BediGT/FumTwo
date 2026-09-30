// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../DataAssets/RealProjectileDataAsset.h"
#include <Math/MathFwd.h>
#include "RealProjectileInterface.generated.h"

UINTERFACE()
class URealProjectileInterface : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IRealProjectileInterface
{
	GENERATED_BODY()

public:
	virtual const URealProjectileDataAsset* GetProjectileProperties() const = 0;
	virtual const FVector GetProjectileVelocity() const = 0;
};
