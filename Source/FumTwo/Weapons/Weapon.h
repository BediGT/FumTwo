// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Enums/EnumWeaponTypes.h"
#include "../Interfaces/Interactable.h"
#include "../Interfaces/AmmoSource.h"
#include "../Interfaces/AutoInteractable.h"
#include "Weapon.generated.h"

class AProjectile;
class UWeaponDA;
class USkeletalMeshComponent;
class USphereComponent;

UCLASS(Blueprintable, BlueprintType)
class FUMTWO_API AWeapon : public AActor, public IInteractable, public IAmmoSource, public IAutoInteractable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	EWeaponType WeaponType = EWeaponType::Unarmed;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TSubclassOf<AProjectile> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<UWeaponDA> WeaponData = nullptr;

	int32 MagAtTheMoment = 1;
	int32 AmmoAtTheMoment = 1;
	int32 MaxAmmo = 1;
	double LastShot = 0.0;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* TriggerSphere = nullptr;

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;
	bool CanFire() const;

public:
	virtual void Tick(float DeltaTime) override;
	
	void Fire(const FVector& Direction, const FVector& Location);
	void Reload();

	void OnAttachment();
	void OnDetachment();

	EWeaponType GetWeaponType() const;
	FString GetWeaponTypeFString() const;

	int32 GetMissingAmmo() const;
	int32 GetMagSize() const;
	int32 GetAmmoSize() const;
	void AddAmmo(int32 AmmoToAdd);
	int32 GetMagAtTheMoment() const;
	void SetMagAtTheMoment(int32 Value);
	int32 GetAmmoAtTheMoment() const;
	void SetAmmoAtTheMoment(int32 Value);
	float GetZoomFov() const;
	UTexture2D* GetReticle() const;

	virtual void Interact(AActor* Interactor) override;
	virtual bool CanInteract(AActor* Interactor) override;
	virtual FString GetInteractionMessage() const override;
	virtual const FVector GetInteractableLocation() const override;

	virtual bool CheckAmmoType(const EWeaponType InWeaponType) const override;
	virtual int32 GetAvailableAmmo(int32 NeededAmmo) override;

	virtual void AutoInteract(AActor* Interactor) override;
};
