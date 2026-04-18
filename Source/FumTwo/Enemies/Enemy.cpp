// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../Components/WeakspotComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->SetCollisionProfileName(TEXT("Hitbox"));
	MeshComponent->SetGenerateOverlapEvents(true);
	
	WeakspotComponent = CreateDefaultSubobject<UWeakspotComponent>(TEXT("Weakspot Component"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::TakeDamage(const UDamageDA& DamageData, const FName& BoneName)
{
	float Damage = DamageData.Damage;

	if (WeakspotComponent->IsBoneWeakspot(BoneName))
		Damage *= DamageData.CriticalMultiplier;
	
	UE_LOG(LogTemp, Error, TEXT("Applied: %f damage"), Damage);
}

