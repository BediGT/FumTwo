// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SettingsWidget.h"

void UMainMenuWidget::NativeConstruct()
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
	
	if (ButtonPlay)
		ButtonPlay->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
	
	if (ButtonSettings)
		ButtonSettings->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsClicked);
	
	if (ButtonQuit)
		ButtonQuit->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
}

void UMainMenuWidget::OnPlayClicked()
{
	this->RemoveFromParent();
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("TestLevel"));
}

void UMainMenuWidget::OnSettingsClicked()
{
	if (WidgetSwitcherMenu)
		WidgetSwitcherMenu->SetActiveWidget(WidgetSettings);
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit,true);
}
