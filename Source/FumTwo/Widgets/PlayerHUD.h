// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UCanvasPanel;
class UTextBlock;
class UProgressBar;
class UImage;
class UTexture2D;

UCLASS()
class FUMTWO_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentWeaponType = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* OtherWeaponType = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Magazine = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Ammo = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Reticle = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* GrenadesCount = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* EquipmentIcon = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InteractionMessage = nullptr;
	
public:
	void SetHealthBarValue(float HealthValue, float MaxHealth) const;

	void UpdateCurrentWeapon(int32 MagAtTheMoment, int32 AmmoAtTheMoment, const FString& Type, UTexture2D* Reticle2D) const;
	void UpdateCurrentWeaponMagazine(int32 MagAtTheMoment) const;
	void UpdateCurrentWeaponAmmo(int32 MagAtTheMoment, int32 AmmoAtTheMoment) const;

	void UpdateOtherWeapon(const FString& Type) const;
	void UpdateGrenades(int32 GrenadesNumber) const;
	void UpdateEquipmentIcon(UTexture2D* Icon) const;
	void UpdateInteractionMessage(const FString& Message) const;
};
