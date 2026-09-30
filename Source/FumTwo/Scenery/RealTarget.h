#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/RealTargetInterface.h"
#include "RealTarget.generated.h"

class UStaticMesh;
class UInstancedStaticMeshComponent;
class UBoxComponent;
class URealMaterialDataAsset;

UCLASS()
class FUMTWO_API ARealTarget : public AActor, public IRealTargetInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Meshes")
	TObjectPtr<UBoxComponent> BoundingBox = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Meshes")
	TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Meshes")
	TObjectPtr<UStaticMesh> ElementStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Meshes")
	FIntVector Dimensions{ 5, 50, 50 };

	FIntVector LastDimensions{ 0, 0, 0};

	UPROPERTY(EditAnywhere, Category = "Physical Properties")
	const URealMaterialDataAsset* Material{};
	
public:	
	ARealTarget();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& OverlapResult
	);

	virtual const URealMaterialDataAsset* GetMaterial() const override;
	virtual const void OnImpact(const FVector& ImpactPoint, const FVector& ImpactDirection, double PenetrationDepth, double Radius) override;
};
