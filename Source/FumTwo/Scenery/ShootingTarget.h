#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingTarget.generated.h"

class UStaticMesh;
class UInstancedStaticMeshComponent;
class UBoxComponent;

UCLASS()
class FUMTWO_API AShootingTarget : public AActor
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
	
public:	
	AShootingTarget();

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
};
