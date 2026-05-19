// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

const UInputMappingContext* AVehicle::GetMappingContext()
{
	return MappingContext;
}

void AVehicle::Interact(AActor* Interactor)
{
	
}

bool AVehicle::CanInteract(AActor* Interactor)
{
	return true;
}

FString AVehicle::GetInteractionMessage() const
{
	return "Press E to enter vehicle";
}

const FVector AVehicle::GetInteractableLocation() const
{
	return GetActorLocation();
}

