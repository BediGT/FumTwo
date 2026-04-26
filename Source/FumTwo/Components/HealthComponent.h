// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../DataAssets/DamageDA.h"
#include "HealthComponent.generated.h"

DECLARE_DELEGATE(FOnZeroHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FUMTWO_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealth = 1;

	UPROPERTY()
	float Health = 1;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxShield = 1;

	UPROPERTY()
	float Shield = 1;

public:
	FOnZeroHealth OnZeroHealthDelegate{};
	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TakeDamage(const UDamageDA* DamageDA, bool bWeakspotHit);
};
