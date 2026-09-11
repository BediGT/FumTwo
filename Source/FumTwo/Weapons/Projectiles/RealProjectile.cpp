// Fill out your copyright notice in the Description page of Project Settings.


#include "RealProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../../Math/Constants.h"

ARealProjectile::ARealProjectile()
	: Super()
{
	StaticMeshComponent->SetMassOverrideInKg(NAME_None, Mass, true);

	CsvData = TEXT("Speed;Vel.X;Vel.Y;Vel.Z\n");
}

void ARealProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateBullet(DeltaTime);
	
	if (bDrawPath)
		DrawPath();
}

void ARealProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& OverlapResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, OverlapResult);

	const FVector ProjectileDirection = GetActorForwardVector();
	const FVector ImpactNormal = OverlapResult.ImpactNormal;

	float ImpactCosine = FMath::Abs(FVector::DotProduct(ProjectileDirection, ImpactNormal));

	const float RicochetCosine = FMath::Cos(FMath::DegreesToRadians(60.f));

	if (ImpactCosine < RicochetCosine)
	{
		// Ricochet
		SetActorLocation(OverlapResult.Location);
		DrawDebugSphere(GetWorld(), GetActorLocation(), 3.f, 1, FColor::Blue, false, 20.f, 0, 1.f);

		FVector NewDirection = FMath::GetReflectionVector(ProjectileDirection, ImpactNormal);
		float Speed = ProjectileMovement->Velocity.Size();
		ProjectileMovement->Velocity = NewDirection * Speed;
	}
	else
	{
		//Penetration
	}
}

void ARealProjectile::OnHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ARealProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FString Directory = FPaths::ProjectSavedDir() / TEXT("Telemetry");
	FString FilePath = Directory / FString::Printf(TEXT("%s.csv"), *GetName());
	FFileHelper::SaveStringToFile(CsvData, *FilePath);
}

void ARealProjectile::UpdateBullet(float DeltaTime)
{
	const float DragForce = 0.5f * AerodynamicCoefficient * Math::AirDensity * CrossSectionArea * GetVelocity().SizeSquared();
	const float DragDeceleration = DragForce / Mass;

	const FVector DragVector = -GetVelocity().GetSafeNormal();
	ProjectileMovement->Velocity += DragVector * DragDeceleration * DeltaTime;

	CsvData += FString::Printf(TEXT("%.2f;%.2f;%.2f;%.2f\n"),
		ProjectileMovement->Velocity.Size(),
		ProjectileMovement->Velocity.X,
		ProjectileMovement->Velocity.Y,
		ProjectileMovement->Velocity.Z
	);
}

void ARealProjectile::DrawPath()
{
	const FVector CurrentPosition = GetActorLocation();

	if (LastPosition.Equals(FVector::ZeroVector, 10e-6))
		LastPosition = CurrentPosition;

	DrawDebugSphere(GetWorld(), CurrentPosition, 1.f, 1, FColor::Red, false, 20.f, 0, 1.f);
	DrawDebugLine(GetWorld(), LastPosition, CurrentPosition, FColor::Green, false, 20.f, 0, 0.5f);
	LastPosition = CurrentPosition;
}
