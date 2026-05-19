// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "UObject/Interface.h"
#include "Possessable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPossessable : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IPossessable
{
	GENERATED_BODY()
public:
	virtual const UInputMappingContext* GetMappingContext() = 0;
};
