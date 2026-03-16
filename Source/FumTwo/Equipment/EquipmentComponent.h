// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Enums/EnumEquipmentType.h"
#include "EquipmentComponent.generated.h"

class AEquipment;
class AEquipmentPickup;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FUMTWO_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<AEquipment> EquipmentClass = {};
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, Category = "Properties")
	EEquipmentType EquipmentType = EEquipmentType::Empty;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TSubclassOf<AEquipmentPickup> EquipmentPickupClass = {};

public:	
	UEquipmentComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UTexture2D* GetIcon() const;
	EEquipmentType GetEquipmentType() const;
	TSubclassOf<AEquipmentPickup> GetEquipmentPickupClass() const;
	void SpawnEquipment(const FTransform& Transform) const;
};
