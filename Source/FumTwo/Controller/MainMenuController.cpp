// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuController.h"
#include "../Widgets/MainMenuWidget.h"

AMainMenuController::AMainMenuController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor  = true;

	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
	
	MainMenuWidget = CreateWidget<UMainMenuWidget>(this, MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}

void AMainMenuController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

