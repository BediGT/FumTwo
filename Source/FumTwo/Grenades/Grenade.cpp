// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->InitSphereRadius(15.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("PhysicsActor"));
	CollisionSphere->SetSimulatePhysics(true);
	CollisionSphere->SetEnableGravity(true);
	CollisionSphere->SetLinearDamping(0.2f);
	CollisionSphere->SetAngularDamping(0.2f);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = 1500.0f;
	ProjectileMovement->MaxSpeed = 1500.0f;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->Friction = 0.5f;
	ProjectileMovement->bShouldBounce = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMesh->SetupAttachment(CollisionSphere);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetRelativeScale3D(FVector(0.1f));

	InitialLifeSpan = 3.0f;
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
