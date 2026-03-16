// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enums/EnumPickupTypes.h"
#include "Pickup.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class FUMTWO_API APickup : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	EPickupType PickupType = {};

	UPROPERTY(EditAnywhere, Category = "Trigger Sphere")
	float InteractionRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Trigger Sphere")
	USphereComponent* TriggerSphere = nullptr;

public:	
	APickup();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	EPickupType GetPickupType() const;
	uint8 GetPickupTypeAsUInt8() const;
};
