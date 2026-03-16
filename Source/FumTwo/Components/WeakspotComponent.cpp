// Fill out your copyright notice in the Description page of Project Settings.


#include "WeakspotComponent.h"

UWeakspotComponent::UWeakspotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeakspotComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWeakspotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UWeakspotComponent::IsBoneWeakspot(const FName& BoneName) const
{
	if (WeakspotBones.Contains(BoneName))
		return true;

	return false;
}


