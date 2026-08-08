#include "MainPlayerCameraManager.h"


AMainPlayerCameraManager::AMainPlayerCameraManager()
	: Super()
{
	DefaultFOV = 98.f;
	UnlockFOV();
}

void AMainPlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();
}
