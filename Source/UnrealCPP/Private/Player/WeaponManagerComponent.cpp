#include "Player/WeaponManagerComponent.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	if(WeaponDatabase.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Database is empty in %s"), *GetOwner()->GetName());
	}
	else
	{
		for (const auto& pair : WeaponDatabase)
		{
			if(!pair.Value || !pair.Value->IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid Weapon Data Asset for Weapon Type %d in %s"), static_cast<int32>(pair.Key), *GetOwner()->GetName());
			}
			else if(pair.Key != pair.Value->WeaponType)
			{
				UE_LOG(LogTemp, Error, TEXT("Mismatched Weapon Code for Weapon Type %d in %s"), static_cast<int32>(pair.Key), *GetOwner()->GetName());
			}
		}
	}
}
