// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FumTwo/Interfaces/Possessable.h"
#include "FumTwo/Camera/MainPlayerCameraManager.h"
#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"

AMainPlayerController::AMainPlayerController()
{
	PlayerCameraManagerClass = AMainPlayerCameraManager::StaticClass();
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Sensitivity = DefaultSensitivity;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerCameraManager->GetClass()->GetName());

	if (!IMC_DefaultMappingContext)
		UE_LOG(LogTemp, Error, TEXT("IMC_GamePlay is not set in AMainPlayerController!"));
	
	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(IMC_DefaultMappingContext, 0);

	if (HUDClass)
	{
		HUD = CreateWidget<UPlayerHUD>(this, HUDClass);
		HUD->AddToPlayerScreen();
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("FOV: %f"), PlayerCameraManager->GetFOVAngle());
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);
}

void AMainPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddYawInput(LookAxisVector.X * Sensitivity);
	AddPitchInput(-LookAxisVector.Y * Sensitivity);
}

void AMainPlayerController::SetZoomedSensitivity(const float& ZoomFov)
{
	if (FMath::IsNearlyEqual(DefaultFov, ZoomFov))
		return;
	
	const float Coefficient = FMath::Tan(FMath::DegreesToRadians(ZoomFov / 2.0f)) / FMath::Tan(FMath::DegreesToRadians(DefaultFov / 2.0f));
	Sensitivity = static_cast<double>(Coefficient) * DefaultSensitivity;
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AMainPlayerController::ZoomIn(const float& ZoomFov)
{
	if (ZoomFov > 0.0f)
	{
		PlayerCameraManager->SetFOV(ZoomFov);
		SetZoomedSensitivity(ZoomFov);
	}
	else
		ResetZoom();
}

void AMainPlayerController::ResetZoom()
{
	PlayerCameraManager->UnlockFOV();
	Sensitivity = DefaultSensitivity;
}

void AMainPlayerController::SwitchPawn(APawn* NewPawn)
{
	if (const auto Possessable = Cast<IPossessable>(NewPawn))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(Possessable->GetMappingContext(), 0);
		}

		ResetZoom();
		Possess(NewPawn);
	}
}
