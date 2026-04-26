// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "../Components/WeakspotComponent.h"
#include "../Components/HealthComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->SetCollisionProfileName(TEXT("Hitbox"));
	MeshComponent->SetGenerateOverlapEvents(true);
	
	WeakspotComponent = CreateDefaultSubobject<UWeakspotComponent>(TEXT("Weakspot Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
		HealthComponent->OnZeroHealthDelegate.BindUObject(this, &AEnemy::Die);
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
	if (HealthComponent)
		HealthComponent->TakeDamage(&DamageData, WeakspotComponent->IsBoneWeakspot(BoneName));
}

void AEnemy::Die() const
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	if (!MeshComponent)
		return;
	
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionProfileName(TEXT("Ragdoll"));
	MeshComponent->WakeAllRigidBodies();
    
	if (HealthComponent)
		HealthComponent->OnZeroHealthDelegate.Unbind();
}

