// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleShield.h"

ABubbleShield::ABubbleShield()
{
	RootComponent = EquipmentMesh;
	
	BubbleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bubble Mesh"));
	BubbleMesh->SetupAttachment(EquipmentMesh);
	BubbleMesh->SetCollisionProfileName("BlockOnlyProjectile");
}

void ABubbleShield::BeginPlay()
{
	Super::BeginPlay();
}

void ABubbleShield::AddVelocity(const FVector& Velocity)
{
	EquipmentMesh->SetPhysicsLinearVelocity(Velocity);
}
