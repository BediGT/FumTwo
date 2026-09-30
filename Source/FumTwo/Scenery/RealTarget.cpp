#include "RealTarget.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../DataAssets/RealMaterialDataAsset.h"
#include "Engine/OverlapResult.h"

ARealTarget::ARealTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounding Box"));
	BoundingBox->SetCollisionProfileName(TEXT("OverlapAll"));
	BoundingBox->OnComponentBeginOverlap.AddDynamic(this, &ARealTarget::OnOverlapBegin);
	RootComponent = BoundingBox;

	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));
	InstancedStaticMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	InstancedStaticMesh->SetCanEverAffectNavigation(false);
	InstancedStaticMesh->SetGenerateOverlapEvents(true);
	InstancedStaticMesh->SetupAttachment(RootComponent);

	//InstancedStaticMesh->bHasPerInstanceHitProxies = false;
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

	FVector Extent{ Dimensions.X * 0.5f, Dimensions.Y * 0.5f, Dimensions.Z * 0.5f };
	BoundingBox->SetBoxExtent(Extent);

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

void ARealTarget::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& OverlapResult)
{
	/*if (!OtherActor || OtherActor == this || !InstancedStaticMesh)
		return;

	int32 HitIndex = OverlapResult.MyItem;

	if (InstancedStaticMesh->IsValidInstance(HitIndex))
	{
		FTransform Transform{};
		InstancedStaticMesh->GetInstanceTransform(HitIndex, Transform);
		Transform.SetScale3D(FVector::ZeroVector);
		InstancedStaticMesh->UpdateInstanceTransform(HitIndex, Transform);
	}*/
}

const URealMaterialDataAsset* ARealTarget::GetMaterial() const
{
	return Material;
}

const void ARealTarget::OnImpact(const FVector& ImpactPoint, const FVector& ImpactDirection, double PenetrationDepth, double Radius)
{
	if (!GetWorld())
		return;

	FVector Direction = ImpactDirection.GetSafeNormal();
	FVector StartTrace = ImpactPoint;
	FVector EndTrace = ImpactPoint + (ImpactDirection * PenetrationDepth);

	auto CylinderShape = FCollisionShape::MakeCapsule(Radius, PenetrationDepth * 0.5f);

	FVector CenterPoint = ImpactPoint + (Direction * (PenetrationDepth * 0.5f));
	FQuat Rotation = Direction.ToOrientationQuat();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;

	TArray<FOverlapResult> OverlapResults{};

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		CenterPoint,
		Rotation,
		ECC_GameTraceChannel1,
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
