// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsSubsystem.h"
#include "JsonObjectConverter.h"

void USettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadSettings();
}

FGameSettings USettingsSubsystem::GetSettings() const
{
	return CurrentSettings;
}

void USettingsSubsystem::ApplySettings(const FGameSettings& NewSettings)
{
	CurrentSettings = NewSettings;
	SaveSettings();
	
	OnSettingsChanged.Broadcast();
}

FString USettingsSubsystem::GetSettingsFilePath()
{
	return FPaths::ProjectSavedDir() / TEXT("Settings.json");
}

void USettingsSubsystem::LoadSettings()
{
	const FString FilePath = GetSettingsFilePath();
	FString JsonString;

	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		SaveSettings();
		return;
	}

	if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &CurrentSettings))
	{
		CurrentSettings = FGameSettings();
		SaveSettings();
	}
}

void USettingsSubsystem::SaveSettings() const
{
	FString JsonString;

	FJsonObjectConverter::UStructToJsonObjectString( FGameSettings::StaticStruct(), &CurrentSettings, JsonString);

	const FString FilePath = GetSettingsFilePath();
	FFileHelper::SaveStringToFile(JsonString, *FilePath);
}
