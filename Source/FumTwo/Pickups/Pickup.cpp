// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionProfileName(TEXT("Pickup"));
	StaticMesh->SetGenerateOverlapEvents(false);

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	TriggerSphere->SetupAttachment(StaticMesh);
	TriggerSphere->SetCollisionProfileName(TEXT("TriggerOnlyPawn"));
	TriggerSphere->SetSphereRadius(InteractionRadius);
	TriggerSphere->SetGenerateOverlapEvents(true);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPickupType APickup::GetPickupType() const
{
	return  PickupType;
}

uint8 APickup::GetPickupTypeAsUInt8() const
{
	return static_cast<uint8>(PickupType);
}
