// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"

AEquipment::AEquipment()
{
	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	EquipmentMesh->SetCollisionProfileName("NoCollision");
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

