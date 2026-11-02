// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;

UCLASS()
class FUMTWO_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ButtonPlay{};
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ButtonSettings{};
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ButtonQuit{};
	
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
