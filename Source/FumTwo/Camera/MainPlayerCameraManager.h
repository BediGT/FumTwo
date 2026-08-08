#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MainPlayerCameraManager.generated.h"

UCLASS()
class FUMTWO_API AMainPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	AMainPlayerCameraManager();

	virtual void BeginPlay() override;
};
