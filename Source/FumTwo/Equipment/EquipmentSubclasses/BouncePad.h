// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Equipment.h"
#include "BouncePad.generated.h"

class UBoxComponent;
class USphereComponent;

UCLASS()
class FUMTWO_API ABouncePad : public AEquipment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* ImpulseSphere;

	UPROPERTY()
	float BounceHeight = 500.0f;
	
public:
	ABouncePad();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void AddVelocity(const FVector& Velocity) override;
};
