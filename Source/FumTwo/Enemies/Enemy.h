// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FumTwo/Components/WeakspotComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UBoxComponent;
class UWeakspotComponent;

UCLASS()
class FUMTWO_API AEnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UWeakspotComponent* WeakspotComponent = nullptr;

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
