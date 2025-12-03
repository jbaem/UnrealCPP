#include "Item/Data/HealthOrbDataAsset.h"

void UHealthOrbDataAsset::Execute(AActor* Target) const
{
	Super::Execute(Target);
	/*
	if (Target && Target->Implements<UHasHealth>())
	{
		IHasHealth::Execute_HealHealth(Target, HealthAmount);
	}
	*/
}
