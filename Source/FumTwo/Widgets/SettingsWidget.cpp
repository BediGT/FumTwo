// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"
#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "../GameSettings/GameSettings.h"
#include "../GameSettings/SettingsSubsystem.h"

void USettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;
	
	LoadGameSettings();
	
	if (ButtonApply)
		ButtonApply->OnClicked.AddDynamic(this, &USettingsWidget::OnApplyClicked);
	
	if (ButtonBack)
		ButtonBack->OnClicked.AddDynamic(this, &USettingsWidget::OnBackClicked);
}

void USettingsWidget::OnApplyClicked()
{
	SaveGameSettings();
}

void USettingsWidget::OnBackClicked()
{
	OnBack.ExecuteIfBound();
}

void USettingsWidget::LoadGameSettings() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
		return;
	
	const auto SettingsSubsystem = GameInstance->GetSubsystem<USettingsSubsystem>();
	if (!SettingsSubsystem)
		return;
	
	const auto& MySettings = SettingsSubsystem->GetSettings();
	
	if (SpinSensitivity)
		SpinSensitivity->SetValue(MySettings.Sensitivity);
	
	if (SpinFov)
		SpinFov->SetValue(MySettings.FieldOfView);
}

void USettingsWidget::SaveGameSettings() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
		return;

	const auto SettingsSubsystem = GameInstance->GetSubsystem<USettingsSubsystem>();
	if (!SettingsSubsystem)
		return;
	
	FGameSettings GameSettings{};
	
	if (SpinSensitivity)
		GameSettings.Sensitivity = SpinSensitivity->GetValue();
	
	if (SpinFov)
		GameSettings.FieldOfView = SpinFov->GetValue();
	
	SettingsSubsystem->ApplySettings(GameSettings);
}
