#include "Player/WeaponManagerComponent.h"

#include "Weapon/Weapon.h"
#include "Player/ActionCharacter.h"
#include "Item/Consumable.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AWeapon* UWeaponManagerComponent::GetEquippedWeaponByItemCode(EItemCode ItemCode) const
{
	return WeaponInstances.Contains(ItemCode) ? WeaponInstances[ItemCode] : nullptr;
}

TSubclassOf<AUsedWeapon> UWeaponManagerComponent::GetUsedWeaponClassByItemCode(EItemCode ItemCode) const
{
	const UWeaponDataAsset* weaponData = WeaponDatabase.Contains(ItemCode) ? WeaponDatabase[ItemCode] : nullptr;
	return weaponData->UsedWeaponClass;
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<AActionCharacter>(GetOwner());

	ValidateWeaponDatabase();
	SpawnWeaponInstances();

	AWeapon* basicWeapon = GetEquippedWeaponByItemCode(EItemCode::EIC_Basic);
	basicWeapon->WeaponActivate(true);

	OwnerPlayer->EquipWeapon(EItemCode::EIC_Basic, 10);
}

void UWeaponManagerComponent::ValidateWeaponDatabase()
{
	if (WeaponDatabase.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Database is empty in %s"), *GetOwner()->GetName());
	}
	else
	{
		for (const auto& pair : WeaponDatabase)
		{
			if (!pair.Value || !pair.Value->IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid Weapon Data Asset for Weapon Type %d in %s"), static_cast<int32>(pair.Key), *GetOwner()->GetName());
			}
			else if (pair.Key != pair.Value->WeaponType)
			{
				UE_LOG(LogTemp, Error, TEXT("Mismatched Weapon Code for Weapon Type %d in %s"), static_cast<int32>(pair.Key), *GetOwner()->GetName());
			}
		}
	}
}

void UWeaponManagerComponent::SpawnWeaponInstances()
{
	WeaponInstances.Empty(WeaponDatabase.Num()); // 미리 할당하여 성능 최적화

	if (!OwnerPlayer.IsValid()) return;

	UWorld* world = GetWorld();
	FVector defaultLocation = FVector(0, 0, -10000);
	for (const auto& pair : WeaponDatabase)
	{
		AWeapon* SpawnedWeapon = world->SpawnActor<AWeapon>(
			pair.Value->EquippedWeaponClass,
			defaultLocation,
			FRotator::ZeroRotator
		);

		if (SpawnedWeapon)
		{
			SpawnedWeapon->AttachToComponent(
				OwnerPlayer->GetMesh(),
				FAttachmentTransformRules::KeepWorldTransform,
				FName("root") // 임시로 root 소켓에 부착
			);
			
			SpawnedWeapon->SetWeaponOwner(Cast<ACharacter>(OwnerPlayer));
			SpawnedWeapon->WeaponActivate(false);

			WeaponInstances.Add(pair.Key, SpawnedWeapon);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon instance for Weapon Type %d in %s"), static_cast<int32>(pair.Key), *GetOwner()->GetName());
		}
	}
}
