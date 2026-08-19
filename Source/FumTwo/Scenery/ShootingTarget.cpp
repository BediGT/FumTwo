#include "ShootingTarget.h"
#include "Components/InstancedStaticMeshComponent.h"

AShootingTarget::AShootingTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	InstancedStatisMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BlockISM"));
	InstancedStatisMesh->SetCollisionProfileName(TEXT("BlockAll"));
	InstancedStatisMesh->OnComponentBeginOverlap.AddDynamic(this, &AShootingTarget::OnOverlapBegin);
	RootComponent = InstancedStatisMesh;
}

void AShootingTarget::BeginPlay()
{
	Super::BeginPlay();
}

void AShootingTarget::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InstancedStatisMesh->ClearInstances();

	if (!ElementStaticMesh)
		return;

	InstancedStatisMesh->SetStaticMesh(ElementStaticMesh);

	for (int32 x = 0; x < Dimensions.X; ++x)
	{
		for (int32 y = 0; y < Dimensions.Y; ++y)
		{
			for (int32 z = 0; z < Dimensions.Z; ++z)
			{
				FVector RelativeLocation(x, y, z);
				FTransform InstanceTransform(RelativeLocation);

				InstancedStatisMesh->AddInstance(InstanceTransform);
			}
		}
	}
}

void AShootingTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShootingTarget::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& OverlapResult)
{
	/*if (!OtherActor || OtherActor == this) return;

	FVector HitLocation = bFromSweep ? static_cast<FVector>(OverlapResult.ImpactPoint) : OtherComp->GetComponentLocation();

	FString DebugText = FString::Printf(TEXT("Overlap: %s | Indeks ISM: %d"), *OtherActor->GetName(), OverlapResult.MyItem);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, DebugText);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugText);

	if (bFromSweep)
	{
		DrawDebugDirectionalArrow(GetWorld(), HitLocation, HitLocation + (OverlapResult.ImpactNormal * 30.0f), 10.0f, FColor::Blue, false, 15.0f, 0, 2.0f);
	}

	DrawDebugString(GetWorld(), HitLocation + FVector(0, 0, 15), DebugText, nullptr, FColor::Yellow, 15.0f, true);

	if (InstancedStatisMesh && OverlapResult.MyItem >= 0)
		InstancedStatisMesh->RemoveInstance(OverlapResult.MyItem);*/
}
