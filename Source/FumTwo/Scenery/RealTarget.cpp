#include "RealTarget.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../DataAssets/RealMaterialDataAsset.h"

ARealTarget::ARealTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	BoundingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounding Box"));
	BoundingBox->SetCollisionProfileName(TEXT("BlockAll"));
	BoundingBox->OnComponentBeginOverlap.AddDynamic(this, &ARealTarget::OnOverlapBegin);
	RootComponent = BoundingBox;

	InstancedStaticMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BlockISM"));
	InstancedStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	InstancedStaticMesh->SetCanEverAffectNavigation(false);
	InstancedStaticMesh->SetupAttachment(RootComponent);

	InstancedStaticMesh->bHasPerInstanceHitProxies = false;
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
	if (!OtherActor || OtherActor == this || !InstancedStaticMesh)
		return;

	int32 HitIndex = OverlapResult.MyItem;

	if (InstancedStaticMesh->IsValidInstance(HitIndex))
	{
		FTransform Transform{};
		InstancedStaticMesh->GetInstanceTransform(HitIndex, Transform);
		Transform.SetScale3D(FVector::ZeroVector);
		InstancedStaticMesh->UpdateInstanceTransform(HitIndex, Transform);
	}
}

const URealMaterialDataAsset* ARealTarget::GetMaterial() const
{
	return Material;
}
