// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Possessable.h"
#include "../Camera/MainPlayerCameraManager.h"
#include "../Widgets/PlayerHUD.h"
#include "../Widgets/PauseMenuWidget.h"
#include "../Components/WeaponManagerComponent.h"
#include "../Grenades/GrenadeComponent.h"
#include "../Player/PlayerCharacter.h"
#include "../GameSettings/SettingsSubsystem.h"

AMainPlayerController::AMainPlayerController()
{
	PlayerCameraManagerClass = AMainPlayerCameraManager::StaticClass();
	OnPossessedPawnChanged.AddDynamic(this, &AMainPlayerController::OnPawnChanged);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SwitchInputToGameplay();

	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		Subsystem->AddMappingContext(DefaultInputMapping, 0);

	if (HUDClass)
	{
		HUD = CreateWidget<UPlayerHUD>(this, HUDClass);
		HUD->AddToPlayerScreen(0);
	}
	
	if (PauseMenuWidgetClass)
	{
		PauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetClass);
		PauseMenuWidget->OnClose.BindLambda([this]
		{
			SwitchInputToGameplay();
		});
		PauseMenuWidget->AddToPlayerScreen(1);
		PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (const auto PossessedPawn = GetPawn())
		SetupPawn(PossessedPawn);
	
	SynchronizeSettings();
	
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		if (const auto SettingsSubsystem = GameInstance->GetSubsystem<USettingsSubsystem>())
			SettingsSubsystem->OnSettingsChanged.AddUObject(this, &AMainPlayerController::SynchronizeSettings);
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent)
		return;
	
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);
	EnhancedInputComponent->BindAction(IA_OpenPauseMenu, ETriggerEvent::Started, this, &AMainPlayerController::OpenPauseMenu);
}

void AMainPlayerController::Look(const FInputActionValue& Value)
{
	const auto& LookAxisVector = Value.Get<FVector2D>();
	AddYawInput(LookAxisVector.X * Sensitivity);
	AddPitchInput(-LookAxisVector.Y * Sensitivity);
}

void AMainPlayerController::SetZoomedSensitivity(const float ZoomFov)
{
	if (!PlayerCameraManager)
		return;
	
	const float DefaultFov = PlayerCameraManager->DefaultFOV;
	if (FMath::IsNearlyEqual(DefaultFov, ZoomFov))
		return;
	
	const float Coefficient = FMath::Tan(FMath::DegreesToRadians(ZoomFov / 2.0f)) / FMath::Tan(FMath::DegreesToRadians(DefaultFov / 2.0f));
	Sensitivity = Coefficient * DefaultSensitivity;
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
	if (!NewPawn)
		return;
	
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

void AMainPlayerController::LeavePawn(APawn* PossessedPawn)
{
	if (!PossessedPawn)
		return;
	
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

void AMainPlayerController::OnPawnChanged(APawn* PreviousPawn, APawn* NextPawn)
{
	LeavePawn(PreviousPawn);
	SetupPawn(NextPawn);
}

void AMainPlayerController::ZoomIn(const float ZoomFov)
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
	const auto Possessable = Cast<IPossessable>(NewPawn);
	if (!Possessable)
		return;
	
	if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(Possessable->GetMappingContext(), 0);
	}

	UnPossess();
	Possess(NewPawn);
}

void AMainPlayerController::SwitchInputToGameplay()
{
	bShowMouseCursor = false;
	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AMainPlayerController::SwitchInputToUI()
{
	bShowMouseCursor = true;
	const FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
}

void AMainPlayerController::SynchronizeSettings()
{
	const UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
		return;
	
	const auto SettingsSubsystem = GameInstance->GetSubsystem<USettingsSubsystem>();
	if (!SettingsSubsystem)
		return;
	
	const FGameSettings& GameSettings = SettingsSubsystem->GetSettings();
	
	DefaultSensitivity = GameSettings.Sensitivity;
	Sensitivity = DefaultSensitivity;
	PlayerCameraManager->DefaultFOV = GameSettings.FieldOfView;
}

void AMainPlayerController::OpenPauseMenu()
{
	if (!PauseMenuWidget)
		return;
	
	SwitchInputToUI();
	PauseMenuWidget->ShowWidget();
}
