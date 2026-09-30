// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../../Interfaces/Damageable.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	StaticMeshComponent->SetNotifyRigidBodyCollision(true);
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = StaticMeshComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = StaticMeshComponent;
	ProjectileMovement->InitialSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bSweepCollision = true;
}

void AProjectile::SetDamageDA(const UDamageDA* NewDamageDA)
{
	DamageDA = NewDamageDA;
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& OverlapResult)
{
	IDamageable* DamageableActor = Cast<IDamageable>(OtherActor);
	if (DamageableActor && DamageDA)
	{
		DamageableActor->TakeDamage(*DamageDA, OverlapResult.BoneName);
		Destroy();
	}
}

void AProjectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UE_LOG(LogTemp, Display, TEXT("HIT SOMETHING"));
		Destroy();
	}
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
