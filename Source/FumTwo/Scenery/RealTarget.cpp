#include "RealTarget.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "../DataAssets/RealMaterialDataAsset.h"
#include "Engine/OverlapResult.h"

ARealTarget::ARealTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));
	InstancedStaticMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	InstancedStaticMesh->SetCanEverAffectNavigation(false);
	InstancedStaticMesh->SetGenerateOverlapEvents(true);
	RootComponent = InstancedStaticMesh;
}

void ARealTarget::BeginPlay()
{
	Super::BeginPlay();
}

void ARealTarget::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Dimensions == LastDimensions && InstancedStaticMesh->GetInstanceCount() > 0)
		return;

	LastDimensions = Dimensions;

	InstancedStaticMesh->ClearInstances();

	if (!ElementStaticMesh)
		return;

	InstancedStaticMesh->SetStaticMesh(ElementStaticMesh);

	for (int32 x = 0; x < Dimensions.X; ++x)
	{
		for (int32 y = 0; y < Dimensions.Y; ++y)
		{
			for (int32 z = 0; z < Dimensions.Z; ++z)
			{
				const FVector RelativeLocation(
					x - (Dimensions.X - 1) * 0.5f,
					y - (Dimensions.Y - 1) * 0.5f,
					z - (Dimensions.Z - 1) * 0.5f
				);

				FTransform InstanceTransform(RelativeLocation);

				InstancedStaticMesh->AddInstance(InstanceTransform);
			}
		}
	}
}

void ARealTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const URealMaterialDataAsset* ARealTarget::GetMaterial() const
{
	return Material;
}

const void ARealTarget::OnImpact(const FVector& ImpactPoint, const FVector& ImpactDirection, double PenetrationDepthInCm, double Radius)
{
	if (!GetWorld())
		return;

	FVector Direction = ImpactDirection.GetSafeNormal();
	FVector StartTrace = ImpactPoint;
	FVector EndTrace = ImpactPoint + (ImpactDirection * PenetrationDepthInCm);

	const auto CylinderShape = FCollisionShape::MakeCapsule(Radius, PenetrationDepthInCm * 0.5f);

	FVector CenterPoint = ImpactPoint + (Direction * (PenetrationDepthInCm * 0.5f));
	FQuat Rotation = FRotationMatrix::MakeFromZ(Direction).ToQuat();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;

	TArray<FOverlapResult> OverlapResults{};

	bool bHit = GetWorld()->OverlapMultiByProfile(
		OverlapResults,
		CenterPoint,
		Rotation,
		TEXT("Projectile"),
		CylinderShape,
		QueryParams
	);

	for (const auto& OverlapeResult : OverlapResults)
	{
		if (OverlapeResult.GetActor() != this || !InstancedStaticMesh->IsValidInstance(OverlapeResult.ItemIndex))
			continue;

		FTransform InstanceTransform{};
		if (InstancedStaticMesh->GetInstanceTransform(OverlapeResult.ItemIndex, InstanceTransform))
		{
			InstanceTransform.SetScale3D(FVector::ZeroVector);
			InstancedStaticMesh->UpdateInstanceTransform(OverlapeResult.ItemIndex, InstanceTransform);
		}
	}
}
