// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayerController.h"
#include "PlayerHUD.h"
#include "../Equipment/EquipmentComponent.h"
#include "../Grenades/GrenadeComponent.h"
#include "../Interfaces/Interactable.h"
#include "../Interfaces/AutoInteractable.h"
#include "../Pickups/EquipmentPickup.h"
#include "../Weapons/Weapon.h"
#include <InputTriggers.h>
#include <Templates/Casts.h>
#include <Templates/SubclassOf.h>
#include <UObject/Object.h>
#include <UObject/UObjectBaseUtility.h>
#include <UObject/UObjectGlobals.h>
#include <Containers/Array.h>
#include <CoreGlobals.h>
#include <Delegates/Delegate.h>
#include <GenericPlatform/GenericPlatformMisc.h>
#include <HAL/Platform.h>
#include <Logging/LogMacros.h>
#include <Math/MathFwd.h>
#include <Components/InputComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Engine/EngineTypes.h>
#include <Engine/HitResult.h>
#include <Engine/World.h>
#include <GameFramework/Actor.h>
#include <GameFramework/Character.h>
#include <cfloat>
#include <FumTwo/Enums/EnumEquipmentType.h>
#include <FumTwo/Enums/EnumWeaponTypes.h>
#include <FumTwo/Interfaces/AmmoSource.h>

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->FieldOfView = FieldOfView;
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	Camera->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MyController = Cast<AMainPlayerController>(Controller);
	
	CurrentWeapon = &PrimaryWeapon;

	if (EquipmentComponentClass)
	{
		EquipmentComponent = NewObject<UEquipmentComponent>(this, EquipmentComponentClass, TEXT("Default Equipment"));
		EquipmentComponent->RegisterComponent();
	}

	if (GrenadesComponentClass)
	{
		GrenadesComponent = NewObject<UGrenadesComponent>(this, GrenadesComponentClass, TEXT("Default Grenades Component"));
		GrenadesComponent->RegisterComponent();
	}

	if (IsValid(MyController) && IsValid(HUDClass))
	{
		HUD = CreateWidget<UPlayerHUD>(MyController, HUDClass);
		HUD->AddToPlayerScreen();
		UpdateWeapons();
		UpdateEquipment();
		UpdateGrenades();
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[APlayerCharacter::BeginPlay] MyController or HUD Class is invalid"));

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlap);
		Capsule->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnEndOverlap);
	}
}


void APlayerCharacter::UpdateNearestInteractable()
{
	if (Interactables.IsEmpty())
		return;

	NearestInteractable = nullptr;
	double MinDistance = DBL_MAX;
	const FVector ActorLocation = GetActorLocation();
	for (const auto& Interactable : Interactables)
	{
		if (!Interactable.IsValid() || !Interactable->CanInteract(this))
			continue;

		const double Distance = FVector::DistSquared(ActorLocation, Interactable->GetInteractableLocation());
		if (Distance < MinDistance || NearestInteractable == nullptr)
		{
			NearestInteractable = Interactable;
			MinDistance = Distance;
		}
	}

	HUD->UpdateInteractionMessage(NearestInteractable.IsValid() ? NearestInteractable->GetInteractionMessage() : L"");
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateNearestInteractable();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &APlayerCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &APlayerCharacter::EndCrouch);
		
		EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &APlayerCharacter::FireCurrentWeapon);
		
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &APlayerCharacter::StartZoomCurrentWeapon);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Completed, this, &APlayerCharacter::EndZoomCurrentWeapon);
		
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &APlayerCharacter::ReloadCurrentWeapon);
		
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		
		EnhancedInputComponent->BindAction(IA_ChangeWeapon, ETriggerEvent::Started, this, &APlayerCharacter::ChangeCurrentWeapon);
		
		EnhancedInputComponent->BindAction(IA_ThrowGrenade, ETriggerEvent::Started, this, &APlayerCharacter::ThrowGrenade);
		
		EnhancedInputComponent->BindAction(IA_UseEquipment, ETriggerEvent::Started, this, &APlayerCharacter::UseEquipment);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[%s] Failed to find an Enhanced Input component!"), *GetNameSafe(this));
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FVector Forward = FVector(Camera->GetComponentRotation().Vector().X, Camera->GetComponentRotation().Vector().Y, 0.0f);
		AddMovementInput(Forward * MovementSpeed, MovementVector.X);
		AddMovementInput(Camera->GetRightVector() * MovementSpeed, MovementVector.Y);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * MyController->GetSensitivity());
		AddControllerPitchInput(-LookAxisVector.Y * MyController->GetSensitivity());
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[%s] m_MyController is invalid!"), *GetNameSafe(this));
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	bool bJumpIsAllowed = GetMovementComponent()->IsJumpAllowed()
		&& (GetMovementComponent()->IsMovingOnGround() || GetMovementComponent()->IsFalling());

	if (bJumpIsAllowed)
	{
		if (!bWasJumping || GetJumpMaxHoldTime() <= 0.0f)
		{
			if (JumpCurrentCount == 0 && GetMovementComponent()->IsFalling())
				bJumpIsAllowed = JumpCurrentCount + 1 < JumpMaxCount;
			else
				bJumpIsAllowed = JumpCurrentCount < JumpMaxCount;
		}
		else
		{
			const bool bJumpKeyHeld = (bPressedJump && JumpKeyHoldTime < GetJumpMaxHoldTime());
			bJumpIsAllowed = bJumpKeyHeld &&
				((JumpCurrentCount < JumpMaxCount) || (bWasJumping && JumpCurrentCount == JumpMaxCount));
		}
	}

	return bJumpIsAllowed;
}

void APlayerCharacter::StartCrouch()
{
	Crouch(false);
}

void APlayerCharacter::EndCrouch()
{
	UnCrouch(false);
}

void APlayerCharacter::FireCurrentWeapon()
{
	if (CurrentWeapon && *CurrentWeapon)
	{
		(*CurrentWeapon)->Fire(Camera->GetComponentTransform());
		UpdateWeapons();
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[%s] Current weapon is invalid!"), *GetNameSafe(this));
}

bool APlayerCharacter::WeaponsNeedAmmo() const
{
	return (PrimaryWeapon && PrimaryWeapon->GetMissingAmmo() > 0) || (SecondaryWeapon && SecondaryWeapon->GetMissingAmmo() > 0);
}

void APlayerCharacter::AddAmmoToWeapons(IAmmoSource* AmmoSource) const
{
	if (PrimaryWeapon)
	{
		const int32 MissingAmmo = PrimaryWeapon->GetMissingAmmo();
		if (MissingAmmo > 0 && AmmoSource->CheckAmmoType(PrimaryWeapon->GetWeaponType()))
			PrimaryWeapon->AddAmmo(AmmoSource->GetAvailableAmmo(MissingAmmo));
	}
	
	if (SecondaryWeapon)
	{
		const int32 MissingAmmo = SecondaryWeapon->GetMissingAmmo();
		if (MissingAmmo > 0 && AmmoSource->CheckAmmoType(SecondaryWeapon->GetWeaponType()))
			SecondaryWeapon->AddAmmo(AmmoSource->GetAvailableAmmo(MissingAmmo));
	}
}

void APlayerCharacter::ReloadCurrentWeapon()
{
	if (!CurrentWeapon || !*CurrentWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Current weapon is invalid!"), *GetNameSafe(this));
		return;
	}

	(*CurrentWeapon)->Reload();

	if (WeaponsNeedAmmo())
	{
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);
		for (AActor* Actor : OverlappingActors)
		{
			if (IAmmoSource* AmmoSource = Cast<IAmmoSource>(Actor))
			{
				AddAmmoToWeapons(AmmoSource);
				if (!WeaponsNeedAmmo())
					break;
			}
		}
	}
		
	UpdateWeapons();
}

void APlayerCharacter::StartZoomCurrentWeapon()
{
	if (!CurrentWeapon || !*CurrentWeapon || !MyController)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Inventory or Controller is invalid!"), *GetNameSafe(this));
		return;
	}

	if (const float ZoomFov = (*CurrentWeapon)->GetZoomFov(); ZoomFov > 0.0f)
	{
		Camera->FieldOfView = ZoomFov;
		MyController->SetZoomSensitivity(FieldOfView, ZoomFov);
	}
	else
		EndZoomCurrentWeapon();
}

void APlayerCharacter::EndZoomCurrentWeapon()
{
	if (!MyController)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Controller is invalid!"), *GetNameSafe(this));
		return;
	}

	Camera->FieldOfView = FieldOfView;
	MyController->ResetSensitivity();
}

void APlayerCharacter::Interact()
{
	if (NearestInteractable)
	{
		NearestInteractable->Interact(this);
		Interactables.Remove(NearestInteractable);
		NearestInteractable = nullptr;
	}
}

void APlayerCharacter::ChangeCurrentWeapon()
{
	if (!PrimaryWeapon || !SecondaryWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] PrimaryWeapon or SecondaryWeapon is invalid!"), *GetNameSafe(this));
		return;
	}

	if (AWeapon** Weapon = GetOtherWeapon())
	{
		CurrentWeapon = Weapon;
		UpdateWeapons();
	}
}

void APlayerCharacter::DetachCurrentWeapon() const
{
	if (AWeapon* Weapon = *CurrentWeapon)
	{
		const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);

		Weapon->DetachFromActor(DetachmentRules);
		Weapon->SetOwner(nullptr);
		Weapon->OnDetachment();

		*CurrentWeapon = nullptr;
	}
}

void APlayerCharacter::AttachWeapon(AWeapon* Weapon)
{
	if (!Weapon)
		return;
	
	const FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true);
	
	Weapon->AttachToComponent(GetCapsuleComponent(), AttachmentRules);
	Weapon->SetOwner(this);
	Weapon->OnAttachment();
	
	*CurrentWeapon = Weapon;
}

AWeapon** APlayerCharacter::GetOtherWeapon()
{
	if (*CurrentWeapon == PrimaryWeapon)
		return &SecondaryWeapon;

	if (*CurrentWeapon == SecondaryWeapon)
		return &PrimaryWeapon;

	return nullptr;
}

void APlayerCharacter::ThrowGrenade()
{ 
	if (!GrenadesComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Grenades component is invalid!"), *GetNameSafe(this));
		return;
	}	

	GrenadesComponent->ThrowGrenade(Camera->GetComponentTransform());
	UpdateGrenades();
}

void APlayerCharacter::UseEquipment()
{
	if (EquipmentComponent && EquipmentComponent->GetEquipmentType() != EEquipmentType::Empty)
	{
		EquipmentComponent->SpawnEquipment(Camera->GetComponentTransform());
		EquipmentComponent->DestroyComponent();
	
		// Create empty equipment
		EquipmentComponent = NewObject<UEquipmentComponent>(this, UEquipmentComponent::StaticClass());
		EquipmentComponent->RegisterComponent();

		UpdateEquipment();
	}
}

void APlayerCharacter::UpdateWeapons()
{
	if (!IsValid(HUD))
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] HUD is invalid!"), *GetNameSafe(this));
		return;
	}

	if (CurrentWeapon && *CurrentWeapon)
	{
		const AWeapon* Weapon = *CurrentWeapon;
		HUD->UpdateCurrentWeapon(Weapon->GetMagAtTheMoment(), Weapon->GetAmmoAtTheMoment(), Weapon->GetWeaponTypeFString(), Weapon->GetReticle());
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[%s] Current weapon is invalid!"), *GetNameSafe(this));

	AWeapon** OtherWeapon = GetOtherWeapon();
	if (OtherWeapon && *OtherWeapon)
		HUD->UpdateOtherWeapon((*OtherWeapon)->GetWeaponTypeFString());
}

void APlayerCharacter::UpdateGrenades() const
{
	if (IsValid(HUD) && GrenadesComponent)
	{
		HUD->UpdateGrenades(GrenadesComponent->GetGrenades());
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[%s] HUD or grenade component is invalid!"), *GetNameSafe(this));
}

void APlayerCharacter::UpdateEquipment() const
{
	if (IsValid(HUD) && EquipmentComponent)
	{
		HUD->UpdateEquipmentIcon(EquipmentComponent->GetIcon());
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[%s] HUD or equipment component is invalid!"), *GetNameSafe(this))
}

void APlayerCharacter::InteractWithWeapon(AActor* Weapon)
{
	if (AWeapon* NewWeapon = Cast<AWeapon>(Weapon))
	{
		AWeapon** NotCurrentWeapon = GetOtherWeapon();
		if (*CurrentWeapon && NotCurrentWeapon && !*NotCurrentWeapon)
			CurrentWeapon = NotCurrentWeapon;
		
		DetachCurrentWeapon();
		AttachWeapon(NewWeapon);

		UpdateWeapons();
	}
}

void APlayerCharacter::InteractWithEquipment(const TSubclassOf<UEquipmentComponent>& EquipmentClass)
{
	UWorld* const World = GetWorld();
	if (World && EquipmentClass && EquipmentComponent)
	{
		if (EquipmentComponent)
		{
			World->SpawnActor<AEquipmentPickup>(EquipmentComponent->GetEquipmentPickupClass(), Camera->GetComponentLocation(), Camera->GetComponentRotation());
			EquipmentComponent->DestroyComponent();
		}
		EquipmentComponent = NewObject<UEquipmentComponent>(this, EquipmentClass);
		EquipmentComponent->RegisterComponent();

		UpdateEquipment();
	}
}

void APlayerCharacter::InteractWithAmmoSource(AActor* AmmoSource)
{
	if (IAmmoSource* Ammo = Cast<IAmmoSource>(AmmoSource))
	{
		AddAmmoToWeapons(Ammo);
		UpdateWeapons();
	}
}

bool APlayerCharacter::CanPickUpWeapon(const EWeaponType WeaponToPickUpType)
{
	if (PrimaryWeapon && WeaponToPickUpType == PrimaryWeapon->GetWeaponType())
		return false;

	if (SecondaryWeapon && WeaponToPickUpType == SecondaryWeapon->GetWeaponType())
		return false;

	return true;
}

bool APlayerCharacter::CanPickUpEquipment(const EEquipmentType EquipmentToPickUpType)
{
	if (EquipmentComponent)
		return EquipmentToPickUpType != EquipmentComponent->GetEquipmentType() && EquipmentToPickUpType != EEquipmentType::Empty;

	return true;
}

void APlayerCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IInteractable* Interactable = Cast<IInteractable>(OtherActor))
		Interactables.Add(Interactable);

	if (IAutoInteractable* AutoInteractable = Cast<IAutoInteractable>(OtherActor))
		AutoInteractable->AutoInteract(this);
}

void APlayerCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IInteractable* Interactable = Cast<IInteractable>(OtherActor))
	{
		Interactables.Remove(Interactable);
		if (Interactables.IsEmpty())
		{
			NearestInteractable = nullptr;
			HUD->UpdateInteractionMessage("");
		}
	}
}
