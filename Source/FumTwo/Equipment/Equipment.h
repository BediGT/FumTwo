// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Equipment.generated.h"

class UStaticMeshComponent;

UCLASS()
class FUMTWO_API AEquipment : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* EquipmentMesh = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	double TurnOnDelay = 0.0;

	bool bIsTurnedOn = false;
	double BeginPlayTime = 0.0;
	
public:	
	AEquipment();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void AddVelocity(const FVector& Velocity);
};
