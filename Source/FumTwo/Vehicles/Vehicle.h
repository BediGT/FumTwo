// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FumTwo/Interfaces/Interactable.h"
#include "FumTwo/Interfaces/Possessable.h"
#include "FumTwo/Interfaces/Passenger.h"
#include "FumTwo/Interfaces/MainController.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

class UStaticMeshComponent;
class USphereComponent;
struct FInputActionValue;
class UCameraComponent;
class UFloatingPawnMovement;
class USpringArmComponent;
class UInputComponent;

UCLASS()
class FUMTWO_API AVehicle : public APawn, public IPossessable, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Static Mesh")
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	USphereComponent* InteractionSphere = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "MovementComponent")
	UFloatingPawnMovement* MovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Interactable")
	float InteractionRadius = 300.0f;

	UPROPERTY(VisibleAnywhere, Category = "Input Mapping Context")
	UInputMappingContext* MappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Drive = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_ExitVehicle = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 1000.0f;

	TScriptInterface<IMainController> DriverController = nullptr;
	TScriptInterface<IPassenger> DriverBody = nullptr;

public:
	AVehicle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void ExitVehicle();

	// Possessable
	virtual const UInputMappingContext* GetMappingContext() const override;

	// Interactable
	virtual void Interact(AActor* Interactor) override;
	virtual bool CanInteract(AActor* Interactor) override;
	virtual FString GetInteractionMessage() const override;
	virtual const FVector GetInteractableLocation() const override;
};
