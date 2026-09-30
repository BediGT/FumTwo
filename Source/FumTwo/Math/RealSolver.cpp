#include "RealSolver.h"
#include "../Interfaces/RealProjectileInterface.h"
#include "../Interfaces/RealTargetInterface.h"
#include <Math/UnrealMathUtility.h>
#include <Math/MathFwd.h>
#include <Engine/Engine.h>

void RealSolver::Solve(const IRealProjectileInterface* Projectile, const IRealTargetInterface* Target, const FHitResult& HitResult)
{
	if (!Projectile || !Target)
		return;

	const URealProjectileDataAsset* ProjectileProperties = Projectile->GetProjectileProperties();
	const URealMaterialDataAsset* TargetMaterial = Target->GetMaterial();

	if (!ProjectileProperties || !TargetMaterial)
		return;

	const URealMaterialDataAsset* ProjectileMaterial = ProjectileProperties->Material;

	if (!ProjectileMaterial)
		return;

	const double TargetResistance =
		TargetMaterial->YieldStrength * (1.1 * FMath::Loge(TargetMaterial->YoungModulus / TargetMaterial->YieldStrength) - ProjectileProperties->ShapeFactor);

	const double CriticalSpeedSquared = 2.0 * FMath::Abs((TargetResistance - ProjectileMaterial->YieldStrength) / ProjectileMaterial->Density);

	const FVector ProjectileVelocity = Projectile->GetProjectileVelocity().GetSafeNormal();
	const double ProjectileSpeed = Projectile->GetProjectileVelocity().Length() * 0.01; // cm -> m
	const double ProjectileSpeedSquared = FMath::Square(ProjectileSpeed);

	const double DensityRatio = TargetMaterial->Density / ProjectileMaterial->Density;

	const double DensityRatioComplement = 1.0 - DensityRatio;

	double PenetrationSpeed = 0.0;

	if (ProjectileSpeedSquared > CriticalSpeedSquared)
		PenetrationSpeed = (ProjectileSpeed - FMath::Sqrt(DensityRatio * ProjectileSpeedSquared + DensityRatioComplement * CriticalSpeedSquared)) / DensityRatioComplement;

	const double RicochetFactor =
		ProjectileMaterial->Density * ProjectileSpeedSquared / TargetResistance * (ProjectileSpeed + PenetrationSpeed) / (ProjectileSpeed - PenetrationSpeed);

	const FVector ImpactNormal = HitResult.ImpactNormal;

	double Dot = FVector::DotProduct(ProjectileVelocity, ImpactNormal);
	double ASq = ProjectileVelocity.SquaredLength();
	double BSq = ImpactNormal.SquaredLength();

	double TanSquared = (ASq * BSq - Dot * Dot) / (Dot * Dot);

	if (TanSquared > RicochetFactor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Orange, TEXT("Ricochet"));
	}
	else if (!FMath::IsNearlyZero(PenetrationSpeed))
	{
		const double Penetration = ProjectileMaterial->Density * ProjectileSpeedSquared * ProjectileProperties->EffectiveLength / (2.0 * TargetResistance);
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Green, FString::Printf(TEXT("Penetration: %lf"), Penetration));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Red, TEXT("Splatter"));
	}
}
