// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interfaces/Damageable.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UBoxComponent;
class UWeakspotComponent;
class UHealthComponent;

UCLASS()
class FUMTWO_API AEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UWeakspotComponent* WeakspotComponent = nullptr;

	UPROPERTY(EditAnywhere)
	UHealthComponent* HealthComponent = nullptr;

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void TakeDamage(const UDamageDA& DamageData, const FName& BoneName) override;

	void Die() const;
};
