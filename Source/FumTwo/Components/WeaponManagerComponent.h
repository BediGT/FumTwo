#pragma once

#include "Components/ActorComponent.h"
#include "../Enums/EnumWeaponTypes.h"
#include "WeaponManagerComponent.generated.h"

class AWeapon;
class IAmmoSource;

DECLARE_DELEGATE_OneParam(FOnShootDelegate, /*MagAmmo*/ int32);
DECLARE_DELEGATE_TwoParams(FOnAmmoChangedDelegate, /*MagAmmo*/ int32, /*AllAmmo*/ int32);
DECLARE_DELEGATE_FourParams(FOnOnCurrentWeaponChangedDelegate, /*Mag*/ int32, /*Ammo*/ int32, /*WeaponName*/ const FString&, /*Reticle*/ UTexture2D*);
DECLARE_DELEGATE_OneParam(FOnOnOtherWeaponChangedDelegate, /*WeaponName*/ const FString&);

UCLASS(meta=(BlueprintSpawnableComponent))
class FUMTWO_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Weapons")
	TSubclassOf<AWeapon> PrimaryWeaponClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	TSubclassOf<AWeapon> SecondaryWeaponClass = nullptr;

	UPROPERTY()
	TObjectPtr<AWeapon> PrimaryWeapon = nullptr;

	UPROPERTY()
	TObjectPtr<AWeapon> SecondaryWeapon = nullptr;

	TObjectPtr<AWeapon>* CurrentWeaponReference = nullptr;

public:
	FOnShootDelegate OnShootDelegate{};
	FOnAmmoChangedDelegate OnAmmoChangedDelegate{};
	FOnOnCurrentWeaponChangedDelegate OnCurrentWeaponChangedDelegate{};
	FOnOnOtherWeaponChangedDelegate OnOtherWeaponChangedDelegate{};

	UWeaponManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	void FireCurrentWeapon(const FVector& Direction, const FVector& Location);
	void ReloadCurrentWeapon();
	void ReplenishAmmo(IAmmoSource* AmmoSource);
	float GetCurrentWeaponZoomFov() const;
	void SwitchCurrentWeapon();
	const AWeapon* GetCurrentWeapon() const;
	const AWeapon* GetOtherWeapon();
	void SwapCurrentWeapon(AWeapon* Weapon);
	bool IsWeaponTypeInLoadout(EWeaponType WeaponType) const;

private:
	bool WeaponsNeedAmmo() const;
	void ReplenishAmmo(IAmmoSource* AmmoSource, TObjectPtr<AWeapon>& Weapon);
	void SearchOverlapsForAmmo();
	TObjectPtr<AWeapon>* GetOtherWeaponReference();
	void DetachWeapon(TObjectPtr<AWeapon>& Weapon);
	bool TryAttachWeapon(AWeapon* Weapon);
	void InitWeapon(TObjectPtr<AWeapon>& Weapon, TSubclassOf<AWeapon> WeaponClass);
};
