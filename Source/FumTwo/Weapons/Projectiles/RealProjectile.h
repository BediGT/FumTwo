// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "../../Interfaces/RealProjectileInterface.h"
#include "../../DataAssets/RealProjectileDataAsset.h"
#include "RealProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class URealProjectileDataAsset;

UCLASS()
class FUMTWO_API ARealProjectile : public AProjectile, public IRealProjectileInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Projectile Properties")
	const TObjectPtr<URealProjectileDataAsset> ProjectileProperties{};

	UPROPERTY(EditAnywhere, Category = "Debug Options")
	bool bDrawPath = false;

	bool bOverlapped = false;

	FVector LastPosition{};

	FString CsvData{};

public:	
	ARealProjectile();

	virtual void Tick(float DeltaTime) override;

	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& OverlapResult
	) override;

	virtual void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateBullet(float DeltaTime);
	void DrawPath();

	virtual const URealProjectileDataAsset* GetProjectileProperties() const override;
	virtual const FVector GetProjectileVelocity() const override;
};
