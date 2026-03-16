// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "../Enums/EnumEquipmentType.h"
#include "../Interfaces/Interactable.h"
#include "EquipmentPickup.generated.h"

class UBoxComponent;
class UEquipmentComponent;

UCLASS()
class FUMTWO_API AEquipmentPickup : public APickup, public IInteractable
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<UEquipmentComponent> EquipmentComponentClass = nullptr;

public:
	AEquipmentPickup();

protected:
	virtual void BeginPlay() override;

public:
	TSubclassOf<UEquipmentComponent> GetEquipmentClass() const;

	EEquipmentType GetEquipmentType() const;

	virtual void Interact(AActor* Interactor) override;
	virtual bool CanInteract(AActor* Interactor) override;
	virtual FString GetInteractionMessage() const override;
	virtual const FVector GetInteractableLocation() const override;

	static void SpawnEquipmentPickup(const UEquipmentComponent* Equipment, const FTransform& TransformationParameters, UWorld* const World);
};
