// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncePad.h"

#include "SWarningOrErrorBox.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

ABouncePad::ABouncePad()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetSimulatePhysics(true);
	CollisionBox->SetCollisionProfileName(TEXT("PhysicsActor"));
	CollisionBox->GetBodyInstance()->bLockXRotation = true;
	CollisionBox->GetBodyInstance()->bLockYRotation = true;
	CollisionBox->GetBodyInstance()->bLockZRotation = true;
	RootComponent = CollisionBox;
	
	EquipmentMesh->SetupAttachment(RootComponent);
	
	ImpulseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	ImpulseSphere->SetupAttachment(RootComponent);
	ImpulseSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ImpulseSphere->SetSphereRadius(100.0f);
	ImpulseSphere->OnComponentBeginOverlap.AddDynamic(this, &ABouncePad::OnBeginOverlap);
}

void ABouncePad::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->SetBoxExtent(EquipmentMesh->GetStaticMesh()->GetBoundingBox().GetExtent());
}

void ABouncePad::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (IsValid(Player))
		Player->LaunchCharacter(FVector(0.0f, 0.0f, 500.0f), false, false);
	else if (IsValid(OtherComp) && OtherComp->IsSimulatingPhysics())
		OtherComp->AddImpulse(FVector(0.0f, 0.0f, 1500.0f * OtherComp->GetMass()));
}

void ABouncePad::AddVelocity(const FVector& Velocity)
{
	CollisionBox->SetPhysicsLinearVelocity(Velocity);
}