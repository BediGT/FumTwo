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
	
	TryInitWeapon(PrimaryWeapon, PrimaryWeaponClass);
	TryInitWeapon(SecondaryWeapon, SecondaryWeaponClass);
}

void UWeaponManagerComponent::FireCurrentWeapon(const FVector& Direction, const FVector& Location)
{
	if (const auto& CurrentWeapon = *CurrentWeaponReference)
		CurrentWeapon->Fire(Direction, Location);
}

void UWeaponManagerComponent::ReloadCurrentWeapon()
{
	if (const auto& CurrentWeapon = *CurrentWeaponReference)
	{
		CurrentWeapon->Reload();
		SearchOverlapsForAmmo();
	}
}

void UWeaponManagerComponent::ReplenishAmmo(IAmmoSource* AmmoSource)
{
	ReplenishAmmo(AmmoSource, PrimaryWeapon);
	ReplenishAmmo(AmmoSource, SecondaryWeapon);
}

float UWeaponManagerComponent::GetCurrentWeaponZoomFov() const
{
	if (const auto& CurrentWeapon = *CurrentWeaponReference)
		return CurrentWeapon->GetZoomFov();

	return 0.f;
}

void UWeaponManagerComponent::SwitchCurrentWeapon()
{
	if (auto OtherWeapon = GetOtherWeapon())
		CurrentWeaponReference = OtherWeapon;
}

const AWeapon* UWeaponManagerComponent::GetCurrentWeapon() const
{
	return *CurrentWeaponReference;
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

TObjectPtr<AWeapon>* UWeaponManagerComponent::GetOtherWeapon()
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
		Weapon->AttachToComponent(AttachmentTarget, AttachmentRules); // If actor gets reals mesh it should be changed to sockets
		Weapon->SetOwner(GetOwner());
		Weapon->OnAttachment();

		return true;
	}

	return false;
}

void UWeaponManagerComponent::TryInitWeapon(TObjectPtr<AWeapon>& Weapon, TSubclassOf<AWeapon> WeaponClass)
{
	if (Weapon || !WeaponClass) // Function can be used only to init weapons at the start, not to change them
		return;

	const auto World = GetWorld();
	if (!World)
		return;

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	Weapon = World->SpawnActor<AWeapon>(WeaponClass, ActorSpawnParams);
	TryAttachWeapon(Weapon);
}
