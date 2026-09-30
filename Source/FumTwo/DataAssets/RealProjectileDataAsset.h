// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RealMaterialDataAsset.h"
#include "RealProjectileDataAsset.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FUMTWO_API URealProjectileDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	const TObjectPtr<URealMaterialDataAsset> Material{};

	UPROPERTY(EditAnywhere)
	double Mass = 1.0;

	UPROPERTY(EditAnywhere)
	double Caliber = 1.0;

	UPROPERTY(EditAnywhere)
	double ShapeFactor = 1.0;

	UPROPERTY(EditAnywhere)
	double AerodynamicCoefficient = 1.0;

	double CrossArea = 1.0;
	double EffectiveLength = 1.0;

private:
	void CalculateProperties();

public:
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
