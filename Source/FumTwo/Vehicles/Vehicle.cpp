// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FumTwo/Interfaces/MainController.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionProfileName("Pawn");
	RootComponent = StaticMesh;

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(FName("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(InteractionRadius);
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

UInputMappingContext* AVehicle::GetMappingContext()
{
	return MappingContext;
}

void AVehicle::Interact(AActor* Interactor)
{
	if (IMainController* MainController = Cast<IMainController>(Interactor->GetInstigatorController()))
		MainController->SwitchPawn(this);
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

