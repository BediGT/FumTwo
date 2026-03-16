// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Enums/EnumWeaponTypes.h"
#include "AmmoSource.generated.h"

UINTERFACE()
class UAmmoSource : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IAmmoSource
{
	GENERATED_BODY()

public:
	virtual bool CheckAmmoType(const EWeaponType WeaponType) const = 0;
	virtual int32 GetAvailableAmmo(int32 NeededAmmoAmount) = 0;
};
