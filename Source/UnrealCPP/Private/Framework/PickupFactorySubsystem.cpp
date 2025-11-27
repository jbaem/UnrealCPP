#include "Framework/PickupFactorySubsystem.h"

#include "Settings/PickupFactorySettings.h"
#include "Item/Pickup.h"
#include "Item/PickupWeapon.h"
#include "Data/WeaponDataAsset.h"
#include "Player/ActionCharacter.h"

void UPickupFactorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UPickupFactorySettings* settings = GetDefault<UPickupFactorySettings>();
	if (!settings) return;

	for(const auto& pair : settings->ItemCodeToDataAsset)
	{
		UWeaponDataAsset* weaponDataAsset = pair.Value.LoadSynchronous();
		if (weaponDataAsset)
		{
			ItemCodeToDataAsset.Add(pair.Key, weaponDataAsset);
		}
	}
}

void UPickupFactorySubsystem::SpawnItem(EItemCode ItemCode, FVector Location, FRotator Rotation, FVector Velocity)
{
	UE_LOG(LogTemp, Warning, TEXT("UPickupFactorySubsystem::SpawnItem - ItemCode: %d"), static_cast<uint8>(ItemCode));
	if (!IsValidItemCode(ItemCode)) return;

	AActor* actor = GetWorld()->SpawnActor<AActor>(
		ItemCodeToDataAsset[ItemCode]->PickupWeaponClass,
		Location,
		Rotation
	);

	if (APickup* WeaponPickUp = Cast<APickup>(actor))
	{
		WeaponPickUp->AddImpulse(Velocity);
	}
}

bool UPickupFactorySubsystem::IsValidItemCode(EItemCode ItemCode)
{
	if (ItemCode == EItemCode::EIC_None ||
		ItemCode == EItemCode::EIC_Basic)
		return false;

	if (!ItemCodeToDataAsset.Contains(ItemCode))
		return false;

	return true;
}

void UPickupFactorySubsystem::SpawnUsedWeaponByItemCode(EItemCode ItemCode, FVector Location, FRotator Rotation)
{
	if (!IsValidItemCode(ItemCode)) return;	

	GetWorld()->SpawnActor<AUsedWeapon>(
		ItemCodeToDataAsset[ItemCode]->UsedWeaponClass,
		Location,
		Rotation
	);
}

APickup* UPickupFactorySubsystem::SpawnCurrentWeaponByItemCode(EItemCode ItemCode, FVector Location, FRotator Rotation, FVector Velocity)
{
	if (!IsValidItemCode(ItemCode)) return nullptr;

	APickup* pickup = GetWorld()->SpawnActor<APickup>(
		ItemCodeToDataAsset[ItemCode]->PickupWeaponClass,
		Location,
		Rotation
	);
	pickup->AddImpulse(Velocity);

	return pickup;
}
