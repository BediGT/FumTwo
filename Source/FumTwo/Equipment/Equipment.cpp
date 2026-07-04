// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"

AEquipment::AEquipment()
{
	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Equipment Mesh"));
	EquipmentMesh->SetSimulatePhysics(true);
	EquipmentMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	EquipmentMesh->GetBodyInstance()->bLockXRotation = true;
	EquipmentMesh->GetBodyInstance()->bLockYRotation = true;
	EquipmentMesh->GetBodyInstance()->bLockZRotation = true;
	RootComponent = EquipmentMesh;
}

void AEquipment::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEquipment::AddVelocity(const FVector& Velocity)
{
}

