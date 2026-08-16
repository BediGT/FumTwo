// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FumTwo/Interfaces/MainController.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

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

	UPROPERTY(EditAnywhere, Category = "Controller Settings")
	float DefaultFov = 98.0f;
	
	double Sensitivity = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Control Settings")
	double DefaultSensitivity = 0.253994f;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Look = nullptr;

	// Head Up Display
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UPlayerHUD> HUDClass = nullptr;

	UPROPERTY()
	TObjectPtr<UPlayerHUD> HUD = nullptr;

public:
	AMainPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Look(const FInputActionValue& Value);
	virtual void SetZoomedSensitivity(const float& ZoomFov);

	void BindWeaponManagerToHUD(UWeaponManagerComponent* WeaponManager);
	void UnbindWeaponManagerFromHUD(UWeaponManagerComponent* WeaponManager);

	void SetupPawn(APawn* Pawn);
	void LeavePawn(APawn* Pawn);

	UFUNCTION()
	void OnPawnChanged(APawn* PreviousPawn, APawn* NextPawn);
	
public:
	virtual void ZoomIn(float ZoomFov) override;
	virtual void ResetZoom() override;
	virtual void SwitchPawn(APawn* NewPawn) override;
};
