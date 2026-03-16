// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AutoInteractable.generated.h"

UINTERFACE()
class UAutoInteractable : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IAutoInteractable
{
	GENERATED_BODY()
	
public:
	virtual void AutoInteract(AActor* Interactor) = 0;
};
