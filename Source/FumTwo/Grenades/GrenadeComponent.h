// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrenadeComponent.generated.h"


class AGrenade;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class FUMTWO_API UGrenadesComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Grenades")
	uint32 Grenades = 1;

	UPROPERTY(EditAnywhere, Category = "Grenades")
	uint32 MaxGrenades = 1;

	UPROPERTY(EditAnywhere, Category = "Grenades")
	TSubclassOf<AGrenade> GrenadeClass = nullptr;

public:
	UGrenadesComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	uint32 GetGrenades() const;
	
	void ThrowGrenade(const FTransform& Transform);

	UFUNCTION()
	void CollectGrenades(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
