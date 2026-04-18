// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../DataAssets/DamageDA.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IDamageable
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(const UDamageDA& DamageData, const FName& BoneName) = 0;
};
