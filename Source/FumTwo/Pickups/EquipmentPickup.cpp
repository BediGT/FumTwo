// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentPickup.h"
#include "../Equipment/EquipmentComponent.h"
#include "FumTwo/Interfaces/Interactor.h"

AEquipmentPickup::AEquipmentPickup()
{
}

void AEquipmentPickup::BeginPlay()
{
	Super::BeginPlay();
	PickupType = EPickupType::Equipment;
}

TSubclassOf<UEquipmentComponent> AEquipmentPickup::GetEquipmentClass() const
{
	return EquipmentComponentClass;
}

EEquipmentType AEquipmentPickup::GetEquipmentType() const
{
	if (IsValid(EquipmentComponentClass))
		return EquipmentComponentClass.GetDefaultObject()->GetEquipmentType();

	return EEquipmentType::Empty;
}

void AEquipmentPickup::Interact(AActor* Interactor)
{
	if (IInteractor* InteractingActor = Cast<IInteractor>(Interactor); InteractingActor && EquipmentComponentClass)
	{
		InteractingActor->InteractWithEquipment(EquipmentComponentClass);
		Destroy();
	}
}

bool AEquipmentPickup::CanInteract(AActor* Interactor)
{
	if (IInteractor* InteractingActor = Cast<IInteractor>(Interactor))
		return InteractingActor->CanPickUpEquipment(GetEquipmentType());
	
	return false;
}

FString AEquipmentPickup::GetInteractionMessage() const
{
	return FString("Press E to pickup equipment");
}

const FVector AEquipmentPickup::GetInteractableLocation() const
{
	return GetActorLocation();
}

void AEquipmentPickup::SpawnEquipmentPickup(const UEquipmentComponent* Equipment, const FTransform& TransformationParameters, UWorld* const World)
{
	if (!IsValid(Equipment) || Equipment->GetEquipmentType() == EEquipmentType::Empty)
		return;
	
	World->SpawnActor<AEquipmentPickup>( Equipment->GetEquipmentPickupClass(), TransformationParameters.GetLocation(), FRotator(TransformationParameters.GetRotation()));
}
