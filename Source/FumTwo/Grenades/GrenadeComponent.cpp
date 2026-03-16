// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeComponent.h"
#include "Grenade.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "FumTwo/Enums/EnumPickupTypes.h"
#include "FumTwo/Pickups/Pickup.h"


UGrenadesComponent::UGrenadesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrenadesComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
		Character->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UGrenadesComponent::CollectGrenades);
}

void UGrenadesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

uint32 UGrenadesComponent::GetGrenades() const
{
	return Grenades;
}

void UGrenadesComponent::ThrowGrenade(const FTransform& Transform)
{
	if (UWorld* const World = GetWorld(); World && IsValid(GrenadeClass) && Grenades > 0)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		World->SpawnActor<AGrenade>(GrenadeClass, Transform.GetLocation(), FRotator(Transform.GetRotation()), ActorSpawnParams);
		Grenades--;
	}
}

void UGrenadesComponent::CollectGrenades(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APickup* Pickup = Cast<APickup>(OtherActor); IsValid(Pickup) && Pickup->GetPickupType() == EPickupType::Grenade && Grenades < MaxGrenades)
	{
		Grenades++;
		Pickup->Destroy();
	}
}
