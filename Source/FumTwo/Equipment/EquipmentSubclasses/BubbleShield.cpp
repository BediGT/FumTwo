// Fill out your copyright notice in the Description page of Project Settings.


#include "BubbleShield.h"
#include "Components/BoxComponent.h"

ABubbleShield::ABubbleShield()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetSimulatePhysics(true);
	CollisionBox->SetCollisionProfileName(TEXT("Pickup"));
	CollisionBox->GetBodyInstance()->bLockXRotation = true;
	CollisionBox->GetBodyInstance()->bLockYRotation = true;
	CollisionBox->GetBodyInstance()->bLockZRotation = true;

	RootComponent = CollisionBox;
	
	EquipmentMesh->SetupAttachment(CollisionBox);
	
	BubbleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BubbleMesh->SetupAttachment(CollisionBox);
	BubbleMesh->SetCollisionProfileName("BlockOnlyProjectile");
}

void ABubbleShield::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->SetBoxExtent(EquipmentMesh->GetStaticMesh()->GetBoundingBox().GetExtent());
}

void ABubbleShield::AddVelocity(const FVector& Velocity)
{
	CollisionBox->SetPhysicsLinearVelocity(Velocity);
}
