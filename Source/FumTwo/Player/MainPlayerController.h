// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FumTwo/Interfaces/MainController.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UInputAction;
struct FInputActionValue;
class UUserWidget;
class UInputMappingContext;

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
	UInputMappingContext* IMC_DefaultMappingContext; 

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Look = nullptr;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	void Look(const FInputActionValue& Value);

	virtual void SetZoomedSensitivity(const float& ZoomFov);

	virtual void OnPossess(APawn* InPawn) override;
	
public:
	virtual void ZoomIn(const float& ZoomFov) override;
	virtual void ResetZoom() override;
};
