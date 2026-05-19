// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FumTwo/Interfaces/Interactable.h"
#include "FumTwo/Interfaces/Possessable.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

UCLASS()
class FUMTWO_API AVehicle : public APawn, public IPossessable, public IInteractable
{
	GENERATED_BODY()

	// Possessable
	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	UInputMappingContext* MappingContext = nullptr;

public:
	AVehicle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Possessable
	virtual const UInputMappingContext* GetMappingContext() override;

	// Interactable
	virtual void Interact(AActor* Interactor) override;
	virtual bool CanInteract(AActor* Interactor) override;
	virtual FString GetInteractionMessage() const override;
	virtual const FVector GetInteractableLocation() const override;
};
