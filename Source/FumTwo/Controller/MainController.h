// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MainController.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UMainController : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IMainController
{
	GENERATED_BODY()

public:
	virtual void ZoomIn(float ZoomFov) = 0;
	virtual void ResetZoom() = 0;
	virtual void SwitchPawn(APawn* Pawn) = 0;
};
