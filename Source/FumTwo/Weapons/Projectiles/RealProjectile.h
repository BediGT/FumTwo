// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "RealProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class FUMTWO_API ARealProjectile : public AProjectile
{
	GENERATED_BODY()

	// Params for cal .338

	UPROPERTY(EditAnywhere, Category = "Physical Properties")
	float AerodynamicCoefficient = 0.3;

	UPROPERTY(EditAnywhere, Category = "Physical Properties")
	float Mass = 0.045; // kg

	UPROPERTY(EditAnywhere, Category = "Physical Properties")
	float CrossSectionArea = 0.578; // cm^2

	FString CsvData{};

public:	
	ARealProjectile();

	virtual void Tick(float DeltaTime) override;

	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& OverlapResult
	);

	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateBullet(float DeltaTime);
};
