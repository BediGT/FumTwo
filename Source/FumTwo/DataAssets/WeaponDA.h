// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DamageDA.h"
#include "WeaponDA.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FUMTWO_API UWeaponDA : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	const UDamageDA* DamageDA = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	int32 MagazineSize = 1;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	int32 AmmunitionSize = 1;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	double Firerate = 0.2;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	double Spread = 0.0;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	int32 Pelets = 1;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	float ReloadTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	float ZoomFov = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	UTexture2D* Reticle = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	USoundWave* Sound = nullptr;
};
