#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSettings.h"
#include "SettingsSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSettingsChanged);

UCLASS()
class FUMTWO_API USettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	FGameSettings CurrentSettings;
	
public:
	FOnSettingsChanged OnSettingsChanged{};

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	FGameSettings GetSettings() const;
	void ApplySettings(const FGameSettings& NewSettings);

private:
	static FString GetSettingsFilePath();
	void LoadSettings();
	void SaveSettings() const;
};