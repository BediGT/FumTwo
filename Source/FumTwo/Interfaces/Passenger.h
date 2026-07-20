

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Passenger.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPassenger : public UInterface
{
	GENERATED_BODY()
};

class FUMTWO_API IPassenger
{
	GENERATED_BODY()

public:
	virtual void OnEnterVehicle() = 0;
	virtual void OnExitVehicle() = 0;
	virtual APawn* GetPawn() = 0;
};
