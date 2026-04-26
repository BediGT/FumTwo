// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	Shield = MaxShield;
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::TakeDamage(const UDamageDA* DamageDA, bool bWeakspotHit)
{
	float Damage = DamageDA->Damage;
	
	if (bWeakspotHit)
		Damage *= DamageDA->CriticalMultiplier;

	if (MaxShield > 0.0f && Shield > 0.0f && DamageDA->AntiShieldMultiplier > 0.0f)
	{
		const float DamageToShield = Damage * DamageDA->AntiShieldMultiplier;
		Damage = DamageToShield > Shield ? Damage - Shield / DamageDA->AntiShieldMultiplier : 0.0f;
		Shield = FMath::Clamp(Shield - DamageToShield, 0.0f, MaxShield);
	}

	if (Shield <= 0.0f && MaxHealth > 0.0f)
		Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	if (Health <= 0.0f)
		OnZeroHealthDelegate.ExecuteIfBound();

	UE_LOG(LogTemp, Error, TEXT("\nHealth: %f"), Health);
	UE_LOG(LogTemp, Error, TEXT("Shield: %f\n"), Shield);
}

