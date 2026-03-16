// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "../Enemies/Enemy.h"
#include "../Components/WeakspotComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->InitSphereRadius(5.0f);
	CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	CollisionSphere->SetCollisionProfileName(TEXT("Projectile"));
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionSphere;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;
	ProjectileMovement->ProjectileGravityScale = Gravity;
	ProjectileMovement->bRotationFollowsVelocity = true;

	// Setup Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(CollisionSphere);
	StaticMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));

	// Scale static mesh to a collision sphere
	StaticMeshComponent->SetRelativeScale3D(FVector(1.0f * CollisionSphere->GetScaledSphereRadius() / 50.0f));
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -CollisionSphere->GetScaledSphereRadius()));

	// Time to Live
	InitialLifeSpan = 10.0f;
}

void AProjectile::SetDamage(const float NewDamage)
{
	if (NewDamage > 0.0f)
		Damage = NewDamage;
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
	if (IsValid(OtherActor))
	{
		float FinalDamage = Damage;

		UWeakspotComponent* WeakspotComp =  OtherActor->FindComponentByClass<UWeakspotComponent>();
		if (WeakspotComp && WeakspotComp->IsBoneWeakspot(OverlapResult.BoneName))
			FinalDamage *= WeakspotMultiplier;

		const FName Name = OverlapResult.BoneName;
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f, Bone name: %s"), FinalDamage, *Name.ToString());
		
		UGameplayStatics::ApplyPointDamage(
			OtherActor, FinalDamage, GetActorForwardVector(), OverlapResult, nullptr, this, UDamageType::StaticClass());
		
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
	if ((OtherActor != nullptr) &&
		(OtherActor != this) &&
		(OtherComp != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT SOMETHING"))
		Destroy();
	}
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
