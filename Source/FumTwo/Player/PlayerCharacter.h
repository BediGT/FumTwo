// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Interactor.h"
#include "FumTwo/Interfaces/Possessable.h"
#include "FumTwo/Interfaces/Passenger.h"
#include "PlayerCharacter.generated.h"

class IMainController;
class IAmmoSource;
class AWeapon;
class IInteractable;
class UGrenadesComponent;
struct FInputActionValue;

class AEquipmentPickup;
class UEquipmentComponent;
class UCameraComponent;
class UInputAction;
class UPickupMappingManager;
class APickup;
class UPlayerHUD;
class UInventoryComponent;
class AGrenade;

UCLASS()
class FUMTWO_API APlayerCharacter : public ACharacter, public IInteractor, public IPossessable, public IPassenger
{
	GENERATED_BODY()

	 IMainController* MyController = nullptr;

	// Camera
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera = nullptr;

	// Input actions
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Jump = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Crouch = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Shoot = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Zoom = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Reload = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Interact = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_ChangeWeapon = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_ThrowGrenade = nullptr;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_UseEquipment = nullptr;
	
	// Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpHeight = 600.0f;
	
	// Head Up Display
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UPlayerHUD> HUDClass = nullptr;

	UPROPERTY()
	UPlayerHUD* HUD = nullptr;

	// Weapons
	UPROPERTY(EditAnywhere, Category = "Weapons")
	AWeapon* PrimaryWeapon = nullptr;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	AWeapon* SecondaryWeapon = nullptr;

	AWeapon** CurrentWeapon = nullptr;

	// Equipment
	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<UEquipmentComponent> EquipmentComponentClass = nullptr;

	UPROPERTY()
	UEquipmentComponent* EquipmentComponent = nullptr;

	// Grenades
	UPROPERTY(EditAnywhere, Category = "Grenades")
	TSubclassOf<UGrenadesComponent> GrenadesComponentClass = nullptr;

	UPROPERTY()
	UGrenadesComponent* GrenadesComponent = nullptr;

	TArray<IInteractable*> OverlappingInteractables{};

	IInteractable* NearestAvailableInteractable = nullptr;

	// Possessable
	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	UInputMappingContext* MappingContext = nullptr;

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual bool CanJumpInternal_Implementation() const override;
	void StartCrouch();
	void EndCrouch();

	// Weapons
	void FireCurrentWeapon();
protected:
	bool WeaponsNeedAmmo() const;
	void AddAmmoToWeapons(IAmmoSource* AmmoSource) const;
public:
	void ReloadCurrentWeapon();
	void ZoomIn();
	void ZoomOut();
	void Interact();
	void ChangeCurrentWeapon();
	void DetachCurrentWeapon() const;
	void AttachWeapon(AWeapon* Weapon);
	AWeapon** GetOtherWeapon();

	// Grenades
	void ThrowGrenade();

	//Equipment
	void UseEquipment();

	// Head Up Display
	void UpdateWeapons();
	void UpdateGrenades() const;
	void UpdateEquipment() const;

	// Interactor
	virtual void InteractWithWeapon(AActor* Weapon) override;
	virtual void InteractWithEquipment(const TSubclassOf<UEquipmentComponent>& EquipmentClass) override;
	virtual void InteractWithAmmoSource(AActor* AmmoSource) override;

	virtual bool CanPickUpWeapon(EWeaponType WeaponToPickUpType) override;
	virtual bool CanPickUpEquipment(EEquipmentType EquipmentToPickUpType) override;

	// Overlap
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Possessable
	virtual const UInputMappingContext* GetMappingContext() const override;

	// Passenger
	virtual void OnEnterVehicle() override;
	virtual void OnExitVehicle() override;
	virtual APawn* GetPawn() override;
};
