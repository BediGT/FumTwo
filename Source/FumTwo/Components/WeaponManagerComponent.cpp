#include "WeaponManagerComponent.h"
#include "../Weapons/Weapon.h"
#include "../Interfaces/AmmoSource.h"
#include <GameFramework/Actor.h>


UWeaponManagerComponent::UWeaponManagerComponent()
{
	CurrentWeaponReference = &PrimaryWeapon;
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitWeapon(PrimaryWeapon, PrimaryWeaponClass);
	if (PrimaryWeapon)
	{
		OnCurrentWeaponChangedDelegate.ExecuteIfBound(
			PrimaryWeapon->GetMagAtTheMoment(),
			PrimaryWeapon->GetAmmoAtTheMoment(),
			PrimaryWeapon->GetWeaponTypeFString(),
			PrimaryWeapon->GetReticle()
		);
	}
	
	InitWeapon(SecondaryWeapon, SecondaryWeaponClass);
	if (SecondaryWeapon)
		OnOtherWeaponChangedDelegate.ExecuteIfBound(SecondaryWeapon->GetWeaponTypeFString());
}

void UWeaponManagerComponent::FireCurrentWeapon(const FVector& Direction, const FVector& Location)
{
	if (const auto& CurrentWeapon = *CurrentWeaponReference)
	{
		CurrentWeapon->Fire(Direction, Location);
		OnShootDelegate.ExecuteIfBound(CurrentWeapon->GetMagAtTheMoment());
	}
}

void UWeaponManagerComponent::ReloadCurrentWeapon()
{
	if (const auto& CurrentWeapon = *CurrentWeaponReference)
	{
		CurrentWeapon->Reload();
		SearchOverlapsForAmmo();
		OnAmmoChangedDelegate.ExecuteIfBound(CurrentWeapon->GetMagAtTheMoment(), CurrentWeapon->GetAmmoAtTheMoment());
	}
}

void UWeaponManagerComponent::ReplenishAmmo(IAmmoSource* AmmoSource)
{
	ReplenishAmmo(AmmoSource, PrimaryWeapon);
	ReplenishAmmo(AmmoSource, SecondaryWeapon);

	if (const auto& CurrentWeapon = *CurrentWeaponReference)
		OnAmmoChangedDelegate.ExecuteIfBound(CurrentWeapon->GetMagAtTheMoment(), CurrentWeapon->GetAmmoAtTheMoment());
}

float UWeaponManagerComponent::GetCurrentWeaponZoomFov() const
{
	if (const auto& CurrentWeapon = *CurrentWeaponReference)
		return CurrentWeapon->GetZoomFov();

	return 0.f;
}

void UWeaponManagerComponent::SwitchCurrentWeapon()
{
	const auto OtherWeaponReference = GetOtherWeaponReference();
	if (OtherWeaponReference && *OtherWeaponReference)
		CurrentWeaponReference = GetOtherWeaponReference();

	if (const auto& CurrentWeapon = *CurrentWeaponReference)
	{
		OnCurrentWeaponChangedDelegate.ExecuteIfBound(
			CurrentWeapon->GetMagAtTheMoment(),
			CurrentWeapon->GetAmmoAtTheMoment(),
			CurrentWeapon->GetWeaponTypeFString(),
			CurrentWeapon->GetReticle()
		);
	}

	if (const auto& OtherWeapon = *GetOtherWeaponReference())
		OnOtherWeaponChangedDelegate.ExecuteIfBound(OtherWeapon->GetWeaponTypeFString());
}

const AWeapon* UWeaponManagerComponent::GetCurrentWeapon() const
{
	return *CurrentWeaponReference;
}

const AWeapon* UWeaponManagerComponent::GetOtherWeapon()
{
	if (const auto OtherWeapon = GetOtherWeaponReference())
		return *OtherWeapon;

	return nullptr;
}

void UWeaponManagerComponent::SwapCurrentWeapon(AWeapon* Weapon)
{
	if (!Weapon)
		return;

	auto& CurrentWeapon = *CurrentWeaponReference;

	if (CurrentWeapon == Weapon)
		return;

	if (TryAttachWeapon(Weapon))
	{
		DetachWeapon(CurrentWeapon);
		CurrentWeapon = Weapon;
	}

	OnCurrentWeaponChangedDelegate.ExecuteIfBound(
		CurrentWeapon->GetMagAtTheMoment(),
		CurrentWeapon->GetAmmoAtTheMoment(),
		CurrentWeapon->GetWeaponTypeFString(),
		CurrentWeapon->GetReticle()
	);
}

bool UWeaponManagerComponent::IsWeaponTypeInLoadout(EWeaponType WeaponType) const
{
	const bool bIsSameAsPrimary = PrimaryWeapon && PrimaryWeapon->GetWeaponType() == WeaponType;
	const bool bIsSameAsSecondary = SecondaryWeapon && SecondaryWeapon->GetWeaponType() == WeaponType;
	return bIsSameAsPrimary || bIsSameAsSecondary;
}

bool UWeaponManagerComponent::WeaponsNeedAmmo() const
{
	const bool bPrimaryNeedsAmmo = PrimaryWeapon && PrimaryWeapon->GetMissingAmmo() > 0;
	const bool bSecondaryNeedsAmmo = SecondaryWeapon && SecondaryWeapon->GetMissingAmmo() > 0;
	return (bPrimaryNeedsAmmo || bSecondaryNeedsAmmo);
}

void UWeaponManagerComponent::ReplenishAmmo(IAmmoSource* AmmoSource, TObjectPtr<AWeapon>& Weapon)
{
	if (!AmmoSource || !Weapon)
		return;

	const int32 MissingAmmo = Weapon->GetMissingAmmo();
	if (MissingAmmo > 0 && AmmoSource->CheckAmmoType(Weapon->GetWeaponType()))
		Weapon->AddAmmo(AmmoSource->GetAvailableAmmo(MissingAmmo));
}

void UWeaponManagerComponent::SearchOverlapsForAmmo()
{
	const auto Owner = GetOwner();
	if (Owner && WeaponsNeedAmmo())
	{
		TArray<AActor*> OverlappingActors;
		Owner->GetOverlappingActors(OverlappingActors);
		for (const auto Actor : OverlappingActors)
		{
			if (const auto AmmoSource = Cast<IAmmoSource>(Actor))
			{
				ReplenishAmmo(AmmoSource);
				if (!WeaponsNeedAmmo())
					break;
			}
		}
	}
}

TObjectPtr<AWeapon>* UWeaponManagerComponent::GetOtherWeaponReference()
{
	return *CurrentWeaponReference == PrimaryWeapon ? &SecondaryWeapon : &PrimaryWeapon;
}

void UWeaponManagerComponent::DetachWeapon(TObjectPtr<AWeapon>& Weapon)
{
	if (!Weapon)
		return;
	
	const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
	Weapon->DetachFromActor(DetachmentRules);
	Weapon->SetOwner(nullptr);
	Weapon->OnDetachment();
}

bool UWeaponManagerComponent::TryAttachWeapon(AWeapon* Weapon)
{
	if (!Weapon)
		return false;

	const auto Owner = GetOwner();
	if (!Owner)
		return false;

	if (const auto AttachmentTarget = Owner->GetRootComponent())
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		Weapon->AttachToComponent(AttachmentTarget, AttachmentRules); // If actor gets real mesh it should be changed to socket
		Weapon->SetOwner(GetOwner());
		Weapon->OnAttachment();

		return true;
	}

	return false;
}

void UWeaponManagerComponent::InitWeapon(TObjectPtr<AWeapon>& Weapon, TSubclassOf<AWeapon> WeaponClass)
{
	if (Weapon || !WeaponClass) // Function can be used only to init weapons at the start, not in other way!
		return;

	const auto World = GetWorld();
	if (!World)
		return;

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Weapon = World->SpawnActor<AWeapon>(WeaponClass, ActorSpawnParams); // It is spawning but in player, it works, but should be reworked
	TryAttachWeapon(Weapon);
}
