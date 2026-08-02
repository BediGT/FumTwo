// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.h"
#include "../Equipment/EquipmentComponent.h"
#include "../Grenades/GrenadeComponent.h"
#include "../Interfaces/Interactable.h"
#include "../Interfaces/AutoInteractable.h"
#include "../Pickups/EquipmentPickup.h"
#include "../Weapons/Weapon.h"
#include "FumTwo/Interfaces/MainController.h"
#include <EnhancedPlayerInput.h>
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
#include <GameFramework/Pawn.h>
#include <GameFramework/PlayerController.h>
#include <FumTwo/Enums/EnumEquipmentType.h>
#include <FumTwo/Enums/EnumWeaponTypes.h>
#include <FumTwo/Interfaces/AmmoSource.h>
#include <FumTwo/Components/WeaponManagerComponent.h>
#include <Math/UnrealMathUtility.h>
#include <UObject/WeakInterfacePtr.h>

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	Camera->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	ACharacter::GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("Weapon Manager"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

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

	MyController = Cast<IMainController>(Controller);

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (IsValid(PlayerController) && IsValid(HUDClass))
	{
		HUD = CreateWidget<UPlayerHUD>(PlayerController, HUDClass);
		HUD->AddToPlayerScreen();
		UpdateWeapons();
		UpdateEquipment();
		UpdateGrenades();
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[APlayerCharacter::BeginPlay] Controller or HUD Class is invalid"));

	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnBeginOverlap);
		Capsule->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnEndOverlap);
	}
}

void APlayerCharacter::UpdateNearestInteractable()
{
	TWeakInterfacePtr<IInteractable> NewInteractable = nullptr;
	double MinDistance = UE_DOUBLE_BIG_NUMBER;
	const FVector ActorLocation = GetActorLocation();

	for (const auto& Interactable : Interactables)
	{
		if (!Interactable.IsValid() || !Interactable->CanInteract(this))
			continue;

		const double Distance = FVector::DistSquared(ActorLocation, Interactable->GetInteractableLocation());
		if (Distance < MinDistance || !NewInteractable.IsValid())
		{
			NewInteractable = Interactable;
			MinDistance = Distance;
		}
	}

	if (NewInteractable != InteractionTarget)
	{
		InteractionTarget = NewInteractable;
		HUD->UpdateInteractionMessage(InteractionTarget.IsValid() ? InteractionTarget->GetInteractionMessage() : L"");
	}
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
		
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &APlayerCharacter::StartCrouch);
		EnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &APlayerCharacter::EndCrouch);
		
		EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &APlayerCharacter::OnShoot);
		
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &APlayerCharacter::ZoomIn);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Completed, this, &APlayerCharacter::ZoomOut);
		
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &APlayerCharacter::ReloadCurrentWeapon);
		
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		
		EnhancedInputComponent->BindAction(IA_ChangeWeapon, ETriggerEvent::Started, this, &APlayerCharacter::SwitchCurrentWeapon);
		
		EnhancedInputComponent->BindAction(IA_ThrowGrenade, ETriggerEvent::Started, this, &APlayerCharacter::ThrowGrenade);
		
		EnhancedInputComponent->BindAction(IA_UseEquipment, ETriggerEvent::Started, this, &APlayerCharacter::UseEquipment);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("[%s] Failed to find an Enhanced Input component!"), *GetNameSafe(this));
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FVector Forward = FVector(Camera->GetComponentRotation().Vector().X, Camera->GetComponentRotation().Vector().Y, 0.0f);
	AddMovementInput(Forward * MovementSpeed, MovementVector.X);
	AddMovementInput(Camera->GetRightVector() * MovementSpeed, MovementVector.Y);
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

void APlayerCharacter::OnShoot()
{
	if (Camera && WeaponManager)
		WeaponManager->FireCurrentWeapon(Camera->GetForwardVector(), Camera->GetComponentLocation());
}

void APlayerCharacter::ReloadCurrentWeapon()
{
	if (!WeaponManager)
		return;

	WeaponManager->ReloadCurrentWeapon();
	UpdateWeapons();
}

void APlayerCharacter::ZoomIn()
{
	if (MyController && WeaponManager)
		MyController->ZoomIn(WeaponManager->GetCurrentWeaponZoomFov());
}

void APlayerCharacter::ZoomOut()
{
	if (MyController)
		MyController->ResetZoom();
}

void APlayerCharacter::Interact()
{
	if (InteractionTarget.IsValid())
		InteractionTarget->Interact(this);
}

void APlayerCharacter::SwitchCurrentWeapon()
{
	if (WeaponManager)
		WeaponManager->SwitchCurrentWeapon();
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
	/*if (!IsValid(HUD))
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
		HUD->UpdateOtherWeapon((*OtherWeapon)->GetWeaponTypeFString());*/
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
	const auto NewWeapon = Cast<AWeapon>(Weapon);
	if (NewWeapon && WeaponManager)
	{
		WeaponManager->SwapCurrentWeapon(NewWeapon);
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
	const auto Ammo = Cast<IAmmoSource>(AmmoSource);
	if (Ammo && WeaponManager)
	{
		WeaponManager->ReplenishAmmo(Ammo);
		UpdateWeapons();
	}
}

bool APlayerCharacter::CanPickUpWeapon(const EWeaponType WeaponToPickUpType)
{
	if (WeaponManager)
		return !WeaponManager->IsWeaponTypeInLoadout(WeaponToPickUpType);

	return false;
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
	if (const auto Interactable = Cast<IInteractable>(OtherActor))
		Interactables.Add(Interactable);

	if (const auto AutoInteractable = Cast<IAutoInteractable>(OtherActor))
		AutoInteractable->AutoInteract(this);
}

void APlayerCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (const auto Interactable = Cast<IInteractable>(OtherActor))
	{
		Interactables.Remove(Interactable);
		UpdateNearestInteractable();
	}
}

const UInputMappingContext* APlayerCharacter::GetMappingContext() const
{
	return MappingContext;
}

void APlayerCharacter::OnEnterVehicle()
{
	if (auto Capsule = GetCapsuleComponent())
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerCharacter::OnExitVehicle()
{
	if (auto Capsule = GetCapsuleComponent())
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

APawn* APlayerCharacter::GetPawn()
{
	return Cast<APawn>(this);
}
