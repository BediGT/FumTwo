// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncePad.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

ABouncePad::ABouncePad()
{
	RootComponent = EquipmentMesh;
	
	ImpulseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Impulse Sphere"));
	ImpulseSphere->SetupAttachment(RootComponent);
	ImpulseSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ImpulseSphere->SetSphereRadius(100.0f);
	ImpulseSphere->OnComponentBeginOverlap.AddDynamic(this, &ABouncePad::OnBeginOverlap);
}

void ABouncePad::BeginPlay()
{
	Super::BeginPlay();
}

void ABouncePad::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
		Player->LaunchCharacter(FVector(0.0f, 0.0f, 500.0f), false, false);
	else if (IsValid(OtherComp) && OtherComp->IsSimulatingPhysics())
		OtherComp->AddImpulse(FVector(0.0f, 0.0f, 1500.0f * OtherComp->GetMass()));
}

void ABouncePad::AddVelocity(const FVector& Velocity)
{
	EquipmentMesh->SetPhysicsLinearVelocity(Velocity);
}