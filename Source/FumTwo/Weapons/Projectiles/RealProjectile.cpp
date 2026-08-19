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
	//DrawDebugLine(GetWorld(), GetActorLocation() - GetVelocity() * DeltaTime, GetActorLocation(), FColor::Green, false, 180.f, 0, 1.5f);
}

void ARealProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& OverlapResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, OverlapResult);

	FString DebugText = FString::Printf(TEXT("Overlap: %s | Index ISM: %d"), *OtherActor->GetName(), OverlapResult.Item);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DebugText);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugText);
	DebugText = FString::Printf(TEXT("Overlap: %s | MyIndex ISM: %d"), *OtherActor->GetName(), OverlapResult.MyItem);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DebugText);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugText);
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
	const float Speed = GetVelocity().Size();

	const float DragForce = 0.5 * AerodynamicCoefficient * Math::AirDensity * CrossSectionArea * Speed * Speed;
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
