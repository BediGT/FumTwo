#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MainPlayerCameraManager.generated.h"

UCLASS()
class FUMTWO_API AMainPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AMainPlayerCameraManager();

protected:
	virtual void BeginPlay() override;
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;
};
