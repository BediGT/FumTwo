#include "RealProjectileDataAsset.h"

void URealProjectileDataAsset::CalculateProperties()
{
	CrossArea = PI * FMath::Square(Caliber * 0.5);
	
	if (Material)
		EffectiveLength = Mass / (CrossArea * Material->Density);
}

void URealProjectileDataAsset::PostLoad()
{
	Super::PostLoad();
	CalculateProperties();
}

#if WITH_EDITOR
void URealProjectileDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	CalculateProperties();
}
#endif
