// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUserWidget;

UCLASS()
class FUMTWO_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Control Settings")
	double Sensitivity = 0.253994;

	double DefaultSensitivity = 0.0f;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* IMC_GamePlay; 

	virtual void BeginPlay() override;
	
public:
	double GetSensitivity() const;
	void SetZoomSensitivity(const float& Fov, const float& ZoomFov);
	void ResetSensitivity();
};
