// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

class UButton;
class USpinBox;

DECLARE_DELEGATE(FOnBackDelegate);

UCLASS()
class FUMTWO_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USpinBox* SpinSensitivity{};
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USpinBox* SpinFov{};
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ButtonApply{};
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ButtonBack{};
	
public:
	FOnBackDelegate OnBack{};
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnApplyClicked();
	
	UFUNCTION()
	void OnBackClicked();
	
	void LoadGameSettings() const;
	void SaveGameSettings() const;
};
