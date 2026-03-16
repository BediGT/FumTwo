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
	
	WeakspotComponent->RegisterComponent();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

