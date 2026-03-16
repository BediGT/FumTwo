// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IInteractable
{
	GENERATED_BODY()
	
public:
	virtual void Interact(AActor* Interactor) = 0;
	virtual bool CanInteract(AActor* Interactor) = 0;
	virtual FString GetInteractionMessage() const = 0;
	virtual const FVector GetInteractableLocation() const = 0;
};
