// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainController.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UPauseMenuWidget;
class IPossessable;
class UInputAction;
struct FInputActionValue;
class UUserWidget;
class UInputMappingContext;
class UPlayerHUD;
class UWeaponManagerComponent;

UCLASS()
class FUMTWO_API AMainPlayerController : public APlayerController, public IMainController
{
	GENERATED_BODY()
	
	float Sensitivity = 1.f;

	UPROPERTY(EditAnywhere, Category = "Control Settings")
	float DefaultSensitivity = 1.f;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Look = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_OpenPauseMenu = nullptr;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPlayerHUD> HUDClass = nullptr;

	UPROPERTY()
	TObjectPtr<UPlayerHUD> HUD = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass = nullptr;
	
	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget = nullptr;

public:
	AMainPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Look(const FInputActionValue& Value);
	virtual void SetZoomedSensitivity(const float ZoomFov);

	void BindWeaponManagerToHUD(UWeaponManagerComponent* WeaponManager);
	static void UnbindWeaponManagerFromHUD(UWeaponManagerComponent* WeaponManager);

	void SetupPawn(APawn* Pawn);
	void LeavePawn(APawn* Pawn);

	UFUNCTION()
	void OnPawnChanged(APawn* PreviousPawn, APawn* NextPawn);
	
public:
	virtual void ZoomIn(float ZoomFov) override;
	virtual void ResetZoom() override;
	virtual void SwitchPawn(APawn* NewPawn) override;
	
private:
	void SwitchInputToGameplay();
	void SwitchInputToUI();
	void SynchronizeSettings();
	void OpenPauseMenu();
};
