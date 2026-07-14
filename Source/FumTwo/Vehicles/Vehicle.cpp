// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "FumTwo/Interfaces/MainController.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionProfileName("Pawn");
	RootComponent = StaticMesh;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(-200.f, 0.f, 150.f));
	Camera->bUsePawnControlRotation = false;

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

void AVehicle::SetupPlayerInputComponent(UInputComponent* VehicleInputComponent)
{
	Super::SetupPlayerInputComponent(VehicleInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(VehicleInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Drive, ETriggerEvent::Triggered, this, &AVehicle::Move);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[%s] Failed to find an Enhanced Input component!"), *GetNameSafe(this));
}

void AVehicle::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Movement component is invalid thats why it's not moving
		// TODO: make it move it move it

		const FVector Forward = FVector(Camera->GetComponentRotation().Vector().X, Camera->GetComponentRotation().Vector().Y, 0.0f);
		AddMovementInput(Forward * MovementSpeed, MovementVector.X);
		AddMovementInput(Camera->GetRightVector() * MovementSpeed, MovementVector.Y);
	}
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

