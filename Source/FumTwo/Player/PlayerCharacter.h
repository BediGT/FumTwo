// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Interactor.h"
#include "FumTwo/Interfaces/Possessable.h"
#include "FumTwo/Interfaces/Passenger.h"
#include <Containers/Array.h>
#include <UObject/WeakInterfacePtr.h>
#include <UObject/ObjectPtr.h>
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

class UWeaponManagerComponent;
class IInteractable;
class UGrenadesComponent;
class AEquipmentPickup;
class UEquipmentComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UInputComponent;

DECLARE_DELEGATE_OneParam(FOnInteractableChangedDelegate, /*NewInteractionMessage*/ const FString&);
DECLARE_DELEGATE_OneParam(FOnEquipmentChangedDelegate, /*NewEquipmentIcon*/ UTexture2D*);

UCLASS()
class FUMTWO_API APlayerCharacter : public ACharacter, public IInteractor, public IPossessable, public IPassenger
{
	GENERATED_BODY()

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

	UPROPERTY(EditAnywhere)
	TObjectPtr <UWeaponManagerComponent> WeaponManager = nullptr;

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

	TArray<TWeakInterfacePtr<IInteractable>> Interactables{};
	TWeakInterfacePtr<IInteractable> InteractionTarget = nullptr;

	// Possessable
	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	UInputMappingContext* MappingContext = nullptr;


public:
	FOnInteractableChangedDelegate OnInteractableChangedDelegate{};
	FOnEquipmentChangedDelegate OnEquipmentChangedDelegate{};

	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	void UpdateNearestInteractable();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	virtual bool CanJumpInternal_Implementation() const override;
	void StartCrouch();
	void EndCrouch();

	void OnShoot();
	void ReloadCurrentWeapon();
	void ZoomIn();
	void ZoomOut();
	void Interact();
	void SwitchCurrentWeapon();

	void ThrowGrenade();

	void UseEquipment();

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
