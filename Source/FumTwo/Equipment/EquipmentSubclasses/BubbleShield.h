// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Equipment.h"
#include "BubbleShield.generated.h"

UCLASS()
class FUMTWO_API ABubbleShield : public AEquipment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* BubbleMesh = nullptr;
	
public:
	ABubbleShield();

protected:
	virtual void BeginPlay() override;

	virtual void AddVelocity(const FVector& Velocity) override;
};
