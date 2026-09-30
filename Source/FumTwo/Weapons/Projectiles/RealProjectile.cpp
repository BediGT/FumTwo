// Fill out your copyright notice in the Description page of Project Settings.


#include "RealProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../../Math/Constants.h"
#include "../../Math/RealSolver.h"
#include "../../Interfaces/RealTargetInterface.h"

ARealProjectile::ARealProjectile()
	: Super()
{
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
	const FHitResult& HitResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, HitResult);

	if (const auto RealTarget = Cast<IRealTargetInterface>(OtherActor))
		RealSolver::Solve(this, RealTarget, HitResult);
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
	if (!ProjectileProperties || !ProjectileMovement)
		return;

	const double SpeedSquared = GetVelocity().SquaredLength() * FMath::Square(0.01); // cm^2 -> m^2
	const double DragForce = 0.5f * ProjectileProperties->AerodynamicCoefficient * Math::AirDensity * ProjectileProperties->CrossArea * SpeedSquared;
	const double DragDeceleration = DragForce / ProjectileProperties->Mass * 100.0; // -> cm^2

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

const URealProjectileDataAsset* ARealProjectile::GetProjectileProperties() const
{
	return ProjectileProperties;
}

const FVector ARealProjectile::GetProjectileVelocity() const
{
	return ProjectileMovement->Velocity;
}
