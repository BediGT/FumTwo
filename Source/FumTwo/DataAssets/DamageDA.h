// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DamageDA.generated.h"

UCLASS()
class FUMTWO_API UDamageDA : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Damage Stats")
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Damage Stats")
	float CriticalMultiplier = 1.0f;
};
