// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Projectiles/Projectile.h"
#include "../Interfaces/Interactor.h"
#include "Kismet/GameplayStatics.h"
#include "../DataAssets/WeaponDA.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetCollisionProfileName(TEXT("Pickup"));
	SkeletalMesh->SetSimulatePhysics(true);
	RootComponent = SkeletalMesh;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(SkeletalMesh);
	TriggerSphere->SetCollisionProfileName(TEXT("TriggerOnlyPawn"));
	TriggerSphere->SetSphereRadius(100.0f);
	TriggerSphere->SetGenerateOverlapEvents(true);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	MagAtTheMoment = WeaponData->MagazineSize;
	AmmoAtTheMoment = WeaponData->AmmunitionSize;
	MaxAmmo = WeaponData->MagazineSize + WeaponData->AmmunitionSize;
}

bool AWeapon::CanFire() const
{
	const double ThisShot = UGameplayStatics::GetTimeSeconds(this);
	return ThisShot - LastShot >= WeaponData->Firerate && MagAtTheMoment > 0;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::Fire(const FVector& Direction, const FVector& Location)
{
	if (ProjectileClass && CanFire())
	{
		if (const auto World = GetWorld())
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			const FRotator Rotation = FMath::VRandCone(Direction, FMath::DegreesToRadians(WeaponData->Spread)).Rotation();
			
			for (int32 i = 0; i < WeaponData->Pelets; ++i)
			{
				const auto Projectile = World->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation, ActorSpawnParams);
				Projectile->SetDamageDA(WeaponData->DamageDA);
			}
			
			MagAtTheMoment--;
			LastShot = UGameplayStatics::GetTimeSeconds(this);
			
			if (WeaponData->Sound)
				UGameplayStatics::PlaySoundAtLocation(this, WeaponData->Sound, Location);
		}
	}
}

void AWeapon::Reload()
{
	if (MagAtTheMoment < WeaponData->MagazineSize && AmmoAtTheMoment > 0)
	{
		const int32 MissingAmmo = WeaponData->MagazineSize - MagAtTheMoment;
		if (MissingAmmo >= AmmoAtTheMoment)
		{
			MagAtTheMoment += AmmoAtTheMoment;
			AmmoAtTheMoment = 0;
		}
		else
		{
			MagAtTheMoment = WeaponData->MagazineSize;
			AmmoAtTheMoment -= MissingAmmo;
		}
	}
}

void AWeapon::OnAttachment()
{
	SetActorEnableCollision(false);
	if (SkeletalMesh)
		SkeletalMesh->SetSimulatePhysics(false);
}

void AWeapon::OnDetachment()
{
	SetActorEnableCollision(true);
	if (SkeletalMesh)
		SkeletalMesh->SetSimulatePhysics(true);
}

EWeaponType AWeapon::GetWeaponType() const
{
	return WeaponType;
}

FString AWeapon::GetWeaponTypeFString() const
{
	return StaticEnum<EWeaponType>()->GetNameStringByValue(static_cast<int64>(WeaponType));
}

int32 AWeapon::GetMissingAmmo() const
{
	return MaxAmmo - AmmoAtTheMoment;
}
 
int32 AWeapon::GetMagSize() const
{
	return WeaponData->MagazineSize;
}

int32 AWeapon::GetAmmoSize() const
{
	return WeaponData->AmmunitionSize;
}

void AWeapon::AddAmmo(const int32 AmmoToAdd)
{
	AmmoAtTheMoment += AmmoToAdd;
}

int32 AWeapon::GetMagAtTheMoment() const
{
	return MagAtTheMoment;
}

void AWeapon::SetMagAtTheMoment(const int32 Value)
{
	MagAtTheMoment = Value;
}

int32 AWeapon::GetAmmoAtTheMoment() const
{
	return AmmoAtTheMoment;
}

void AWeapon::SetAmmoAtTheMoment(const int32 Value)
{
	AmmoAtTheMoment = Value;
}

float AWeapon::GetZoomFov() const
{
	return WeaponData->ZoomFov;
}

UTexture2D* AWeapon::GetReticle() const
{
	return WeaponData->Reticle;
}

void AWeapon::Interact(AActor* Interactor)
{
	if (IInteractor* InteractingActor = Cast<IInteractor>(Interactor))
		InteractingActor->InteractWithWeapon(this);
}

bool AWeapon::CanInteract(AActor* Interactor)
{
	if (IInteractor* InteractingActor = Cast<IInteractor>(Interactor))
		return InteractingActor->CanPickUpWeapon(WeaponType);

	return false;
}

FString AWeapon::GetInteractionMessage() const
{
	return FString("Press E to pickup " + GetWeaponTypeFString());
}

const FVector AWeapon::GetInteractableLocation() const
{
	return GetActorLocation();
}

bool AWeapon::CheckAmmoType(const EWeaponType InWeaponType) const
{
	return InWeaponType == WeaponType;
}

int32 AWeapon::GetAvailableAmmo(const int32 NeededAmmo)
{
	const int32 AmmoToReturn = FMath::Min(AmmoAtTheMoment, NeededAmmo);
	AmmoAtTheMoment -= AmmoToReturn;
	return AmmoToReturn;
}

void AWeapon::AutoInteract(AActor* Interactor)
{
	if (IInteractor* InteractingActor = Cast<IInteractor>(Interactor))
		InteractingActor->InteractWithAmmoSource(this);
}
