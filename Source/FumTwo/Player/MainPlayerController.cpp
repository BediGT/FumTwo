// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FumTwo/Interfaces/Possessable.h"
#include "FumTwo/Camera/MainPlayerCameraManager.h"
#include "PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "../Components/WeaponManagerComponent.h"
#include "../Grenades/GrenadeComponent.h"
#include "PlayerCharacter.h"

AMainPlayerController::AMainPlayerController()
{
	PlayerCameraManagerClass = AMainPlayerCameraManager::StaticClass();
	OnPossessedPawnChanged.AddDynamic(this, &AMainPlayerController::OnPawnChanged);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Sensitivity = DefaultSensitivity;
	PlayerCameraManager->DefaultFOV = DefaultFov;

	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(IMC_DefaultMappingContext, 0);

	if (HUDClass)
	{
		HUD = CreateWidget<UPlayerHUD>(this, HUDClass);
		HUD->AddToPlayerScreen();
	}

	if (const auto PossessedPawn = GetPawn())
		SetupPawn(PossessedPawn);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);
}

void AMainPlayerController::Look(const FInputActionValue& Value)
{
	const auto& LookAxisVector = Value.Get<FVector2D>();
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

void AMainPlayerController::BindWeaponManagerToHUD(UWeaponManagerComponent* WeaponManager)
{
	if (!WeaponManager || !HUD)
		return;

	WeaponManager->OnShootDelegate.BindUObject(HUD, &UPlayerHUD::UpdateCurrentWeaponMagazine);
	WeaponManager->OnAmmoChangedDelegate.BindUObject(HUD, &UPlayerHUD::UpdateCurrentWeaponAmmo);
	WeaponManager->OnCurrentWeaponChangedDelegate.BindUObject(HUD, &UPlayerHUD::UpdateCurrentWeapon);
	WeaponManager->OnOtherWeaponChangedDelegate.BindUObject(HUD, &UPlayerHUD::UpdateOtherWeapon);
}

void AMainPlayerController::UnbindWeaponManagerFromHUD(UWeaponManagerComponent* WeaponManager)
{
	if (!WeaponManager)
		return;

	WeaponManager->OnShootDelegate.Unbind();
	WeaponManager->OnAmmoChangedDelegate.Unbind();
	WeaponManager->OnCurrentWeaponChangedDelegate.Unbind();
	WeaponManager->OnOtherWeaponChangedDelegate.Unbind();
}

void AMainPlayerController::SetupPawn(APawn* NewPawn)
{
	if (NewPawn)
	{
		if (const auto WeaponManager = NewPawn->FindComponentByClass<UWeaponManagerComponent>())
			BindWeaponManagerToHUD(WeaponManager);

		if (const auto GrenadesComponent = NewPawn->FindComponentByClass<UGrenadesComponent>())
			GrenadesComponent->OnGrenadesChangedDelegate.BindUObject(HUD, &UPlayerHUD::UpdateGrenades);

		if (const auto PlayerCharacter = Cast<APlayerCharacter>(NewPawn))
		{
			PlayerCharacter->OnInteractableChangedDelegate.BindUObject(HUD, &UPlayerHUD::UpdateInteractionMessage);
			PlayerCharacter->OnEquipmentChangedDelegate.BindUObject(HUD, &UPlayerHUD::UpdateEquipmentIcon);
		}
	}
}

void AMainPlayerController::LeavePawn(APawn* PossessedPawn)
{
	if (PossessedPawn)
	{
		ResetZoom();

		if (const auto WeaponManager = PossessedPawn->FindComponentByClass<UWeaponManagerComponent>())
			UnbindWeaponManagerFromHUD(WeaponManager);

		if (const auto GrenadesComponent = PossessedPawn->FindComponentByClass<UGrenadesComponent>())
			GrenadesComponent->OnGrenadesChangedDelegate.Unbind();

		if (const auto PlayerCharacter = Cast<APlayerCharacter>(PossessedPawn))
		{
			PlayerCharacter->OnInteractableChangedDelegate.Unbind();
			PlayerCharacter->OnEquipmentChangedDelegate.Unbind();
		}
	}
}

void AMainPlayerController::OnPawnChanged(APawn* PreviousPawn, APawn* NextPawn)
{
	LeavePawn(PreviousPawn);
	SetupPawn(NextPawn);
}

void AMainPlayerController::ZoomIn(float ZoomFov)
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

		UnPossess();
		Possess(NewPawn);
	}
}
