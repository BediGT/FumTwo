// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class USettingsWidget;
class UWidgetSwitcher;
class UButton;

DECLARE_DELEGATE(FOnCloseDelegate);

UCLASS()
class FUMTWO_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcherMenu;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USettingsWidget> WidgetSettings{};
	
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonResume{};
	
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonSettings{};
	
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonQuitToMenu{};
	
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonQuit{};
	
public:
	FOnCloseDelegate OnClose{};
	
	void ShowWidget();
	 
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	
private:
	UFUNCTION()
	void OnResumeClicked();
	
	UFUNCTION()
	void OnSettingsClicked();
	
	UFUNCTION()
	void OnQuitToMenuClicked();
	
	UFUNCTION()
	void OnQuitClicked();
};
