#pragma once

#include "Components/ActorComponent.h"
#include "../Enums/EnumWeaponTypes.h"
#include "WeaponManagerComponent.generated.h"

class AWeapon;
class IAmmoSource;

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
