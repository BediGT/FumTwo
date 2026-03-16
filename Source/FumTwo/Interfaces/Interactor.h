// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FumTwo/Enums/EnumEquipmentType.h"
#include "FumTwo/Enums/EnumWeaponTypes.h"
#include "UObject/Interface.h"
#include "Interactor.generated.h"

class UEquipmentComponent;

// This class does not need to be modified.
UINTERFACE()
class UInteractor : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IInteractor
{
	GENERATED_BODY()
	
public:
	virtual void InteractWithWeapon(AActor* Weapon) = 0;
	virtual void InteractWithEquipment(const TSubclassOf<UEquipmentComponent>& EquipmentClass) = 0;
	virtual void InteractWithAmmoSource(AActor* AmmoSource) = 0;
	virtual bool CanPickUpWeapon(EWeaponType WeaponToPickUpType) = 0;
	virtual bool CanPickUpEquipment(EEquipmentType EquipmentToPickUpType) = 0;
};
