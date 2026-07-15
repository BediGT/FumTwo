// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "FumTwo/Interfaces/MainController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionProfileName("Pawn");
	RootComponent = StaticMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 75.f);
	SpringArm->TargetOffset = FVector(0.f, 0.f, 50.f);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>("InteractionSphere");
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(InteractionRadius);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
	MovementComponent->SetUpdatedComponent(RootComponent);
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

	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(VehicleInputComponent))
		EnhancedInputComponent->BindAction(IA_Drive, ETriggerEvent::Triggered, this, &AVehicle::Move);
	else
		UE_LOG(LogTemp, Error, TEXT("[%s] Failed to find an Enhanced Input component!"), *GetNameSafe(this));
}

void AVehicle::Move(const FInputActionValue& Value)
{
	if (Controller)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		const FVector Forward = FVector(Camera->GetComponentRotation().Vector().X, Camera->GetComponentRotation().Vector().Y, 0.f);
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
	if (auto MainController = Cast<IMainController>(Interactor->GetInstigatorController()))
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

