#include "MainPlayerCameraManager.h"
#include "Math/UnrealMathUtility.h"

AMainPlayerCameraManager::AMainPlayerCameraManager()
	: Super()
{
	DefaultFOV = 69.f;
}

void AMainPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTargetInternal(OutVT, DeltaTime);

	OutVT.POV.FOV = DefaultFOV;
	OutVT.POV.DesiredFOV = DefaultFOV;
}
