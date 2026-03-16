// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Equipment.h"
#include "../Pickups/EquipmentPickup.h"

UEquipmentComponent::UEquipmentComponent()
{
	if (const ConstructorHelpers::FObjectFinder<UTexture2D> EmptyIcon(TEXT("/Game/HudIcons/Transparent120x120.Transparent120x120")); EmptyIcon.Succeeded())
		Icon = EmptyIcon.Object;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UTexture2D* UEquipmentComponent::GetIcon() const
{
	return Icon;
}

EEquipmentType UEquipmentComponent::GetEquipmentType() const
{
	return EquipmentType;
}

TSubclassOf<AEquipmentPickup> UEquipmentComponent::GetEquipmentPickupClass() const
{
	return EquipmentPickupClass;
}

void UEquipmentComponent::SpawnEquipment(const FTransform& Transform) const
{
	if (UWorld* const World = GetOwner()->GetWorld())
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		// AEquipment* SpawnedEquipment = World->SpawnActor<AEquipment>(
		// 	EquipmentClass, GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector().GetSafeNormal() * 150.0),
		// 	{0.0, 0.0, 0.0}, ActorSpawnParams);
		// SpawnedEquipment->AddVelocity(GetOwner()->GetActorForwardVector().GetSafeNormal() * 500.0);

		AEquipment* SpawnedEquipment = World->SpawnActor<AEquipment>(
			EquipmentClass, Transform.GetLocation() + (Transform.Rotator().Vector().GetSafeNormal() * 150.0), {0.0, 0.0, 0.0}, ActorSpawnParams);
		SpawnedEquipment->AddVelocity(Transform.Rotator().Vector().GetSafeNormal() * 500.0);
	}
}
