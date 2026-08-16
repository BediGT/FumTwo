// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UPlayerHUD::SetHealthBarValue(const float HealthValue, const float MaxHealth) const
{
	if (HealthBar)
		HealthBar->SetPercent(HealthValue / MaxHealth);
}

void UPlayerHUD::UpdateCurrentWeapon(int32 MagAtTheMoment, int32 AmmoAtTheMoment, const FString& Type, UTexture2D* Reticle2D) const
{
	if (Magazine)
		Magazine->SetText(FText::FromString(FString::FromInt(MagAtTheMoment)));

	if (Ammo)
		Ammo->SetText(FText::FromString(FString::FromInt(AmmoAtTheMoment)));

	if (CurrentWeaponType)
		CurrentWeaponType->SetText(FText::FromString(Type));

	if (Reticle)
		Reticle->SetBrushFromTexture(Reticle2D);
}

void UPlayerHUD::UpdateCurrentWeaponMagazine(int32 MagAtTheMoment) const
{
	if (Magazine)
		Magazine->SetText(FText::FromString(FString::FromInt(MagAtTheMoment)));
}

void UPlayerHUD::UpdateCurrentWeaponAmmo(int32 MagAtTheMoment, int32 AmmoAtTheMoment) const
{
	if (Magazine)
		Magazine->SetText(FText::FromString(FString::FromInt(MagAtTheMoment)));

	if (Ammo)
		Ammo->SetText(FText::FromString(FString::FromInt(AmmoAtTheMoment)));
}

void UPlayerHUD::UpdateOtherWeapon(const FString& Type) const
{
	if (OtherWeaponType)
		OtherWeaponType->SetText(FText::FromString(Type));
}

void UPlayerHUD::UpdateGrenades(int32 GrenadesNumber) const
{
	if (GrenadesCount)
		GrenadesCount->SetText(FText::FromString(FString::FromInt(GrenadesNumber)));
}

void UPlayerHUD::UpdateEquipmentIcon(UTexture2D* Icon) const
{
	if (EquipmentIcon)
		EquipmentIcon->SetBrushFromTexture(Icon);
}

void UPlayerHUD::UpdateInteractionMessage(const FString& Message) const
{
	if (InteractionMessage)
		InteractionMessage->SetText(FText::FromString(Message));
}

