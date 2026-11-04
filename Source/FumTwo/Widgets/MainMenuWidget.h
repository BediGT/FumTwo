// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;
class UButton;
class USettingsWidget;

UCLASS()
class FUMTWO_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcherMenu;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USettingsWidget> WidgetSettings{};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonQuit{};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonPlay{};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonSettings{};
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnPlayClicked();
	
	UFUNCTION()
	void OnSettingsClicked();
	
	UFUNCTION()
	void OnQuitClicked();
};
