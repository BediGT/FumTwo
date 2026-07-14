// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"../DataAssets/DamageDA.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class FUMTWO_API AProjectile : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Projectile Stats")
	float MovementSpeed = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile Stats")
	float Gravity = 0.0f;

	UPROPERTY()
	const UDamageDA* DamageDA = nullptr;

public:	

	AProjectile();

	void SetDamageDA(const UDamageDA* NewDamageDA);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& OverlapResult
	);

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
