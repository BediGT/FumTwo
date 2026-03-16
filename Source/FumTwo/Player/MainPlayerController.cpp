// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	DefaultSensitivity = Sensitivity;

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(IMC_GamePlay, 0);
	}

	if (!IMC_GamePlay)
	{
		UE_LOG(LogTemp, Error, TEXT("IMC_GamePlay is not set in AMainPlayerController!"));
	}
}

double AMainPlayerController::GetSensitivity() const
{
	return Sensitivity;
}

void AMainPlayerController::SetZoomSensitivity(const float& Fov, const float& ZoomFov)
{
	if (FMath::IsNearlyEqual(Fov, ZoomFov)) return;
	const float Coefficient = FMath::Tan(FMath::DegreesToRadians(ZoomFov / 2.0f)) / FMath::Tan(FMath::DegreesToRadians(Fov / 2.0f));
	Sensitivity = static_cast<double>(Coefficient) * DefaultSensitivity;
}

void AMainPlayerController::ResetSensitivity()
{
	Sensitivity = DefaultSensitivity;
}
