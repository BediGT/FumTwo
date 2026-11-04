// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"

#include "SettingsWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::ShowWidget()
{
	SetVisibility(ESlateVisibility::Visible);
	SetFocus();
	
	if (WidgetSwitcherMenu)
		WidgetSwitcherMenu->SetActiveWidgetIndex(0);
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsFocusable = true;
	
	if (WidgetSettings)
	{
		WidgetSettings->OnBack.BindLambda([this]
		{
			if (WidgetSwitcherMenu)
				WidgetSwitcherMenu->SetActiveWidgetIndex(0);
		});
	}
	
	if (ButtonResume)
		ButtonResume->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);
	
	if (ButtonSettings)
		ButtonSettings->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSettingsClicked);
	
	if (ButtonQuitToMenu)
		ButtonQuitToMenu->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitToMenuClicked);
	
	if (ButtonQuit)
		ButtonQuit->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
}

FReply UPauseMenuWidget::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Tab)
	{
		OnResumeClicked();
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(MyGeometry, InKeyEvent);
}

void UPauseMenuWidget::OnResumeClicked()
{
	SetVisibility(ESlateVisibility::Hidden);
	OnClose.ExecuteIfBound();
}

void UPauseMenuWidget::OnSettingsClicked()
{
	if (WidgetSwitcherMenu)
		WidgetSwitcherMenu->SetActiveWidget(WidgetSettings);
}

void UPauseMenuWidget::OnQuitToMenuClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

void UPauseMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit,true);
}
