#include "Weapon/Manager/WeaponManagerComponent.h"

#include "Core/Interface/Consumable.h"
#include "Item/SpawnSystem/PickupFactorySubsystem.h"
#include "Item/Pickupable/PickupWeapon.h"
#include "Player/Base/ActionCharacter.h"
#include "Weapon/Base/Weapon.h"

UWeaponManagerComponent::UWeaponManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	InitOwner();
	ValidateWeaponDatabase();
	SpawnWeaponInstances();
	InitializeEquippedWeapon();
}

AWeapon* UWeaponManagerComponent::GetEquippedWeaponByItemCode(EItemCode ItemCode) const
{
	return WeaponInstances.Contains(ItemCode) ? WeaponInstances[ItemCode] : nullptr;
}

TSubclassOf<AUsedWeapon> UWeaponManagerComponent::GetUsedWeaponClassByItemCode(EItemCode ItemCode) const
{
	const UWeaponDataAsset* WeaponDataAsset = WeaponDB.Contains(ItemCode) ? WeaponDB[ItemCode] : nullptr;
	return WeaponDataAsset->UsedWeaponClass;
}

TSubclassOf<APickupWeapon> UWeaponManagerComponent::GetPickupWeaponClassByItemCode(EItemCode ItemCode) const
{
	const UWeaponDataAsset* WeaponDataAsset = WeaponDB.Contains(ItemCode) ? WeaponDB[ItemCode] : nullptr;
	return WeaponDataAsset->PickupWeaponClass;
}

void UWeaponManagerComponent::InitOwner()
{
	Owner = Cast<AActionCharacter>(GetOwner());
}

void UWeaponManagerComponent::InitializeEquippedWeapon()
{
	AWeapon* BasicWeapon = GetEquippedWeaponByItemCode(EItemCode::EIC_Basic);
	BasicWeapon->WeaponActivate(true);
	EquipWeapon(EItemCode::EIC_Basic, 10);
}

void UWeaponManagerComponent::ValidateWeaponDatabase()
{
	if (WeaponDB.Num() <= 0) return;
	
	for (const auto& pair : WeaponDB)
	{
		if (!pair.Value || !pair.Value->IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid Weapon Data Asset for Weapon Type %d in %s"), static_cast<int32>(pair.Key), *GetOwner()->GetName());
		}
		else if (pair.Key != pair.Value->ItemCode)
		{
			UE_LOG(LogTemp, Error, TEXT("Mismatched Weapon Code for Weapon Type %d in %s"), static_cast<int32>(pair.Key), *GetOwner()->GetName());
		}
	}
}

void UWeaponManagerComponent::SpawnWeaponInstances()
{
	if (!Owner.IsValid()) return;

	WeaponInstances.Empty(WeaponDB.Num()); // 미리 할당하여 성능 최적화
	for (const auto& pair : WeaponDB)
	{
		AWeapon* SpawnedWeapon = SpawnWeaponInstance(pair.Value);
		AttachInstanceToOwner(SpawnedWeapon, pair.Key);
	}
}

AWeapon* UWeaponManagerComponent::SpawnWeaponInstance(UWeaponDataAsset* InDataAsset)
{
	if (!InDataAsset) return nullptr;

	FVector StorageLocation = FVector(0, 0, -10000);
	return GetWorld()->SpawnActor<AWeapon>(
		InDataAsset->EquippedWeaponClass,
		StorageLocation,
		FRotator::ZeroRotator
	);
}

void UWeaponManagerComponent::AttackEnable(bool Flag)
{
	if (!WeaponCurrent) return;
	WeaponCurrent->AttackEnable(Flag);
}

void UWeaponManagerComponent::ActivateSlashEffect(bool Flag)
{
	if (!WeaponCurrent) return;
	WeaponCurrent->ActivateSlashEffect(Flag);
}

bool UWeaponManagerComponent::CanAttack()
{
	if (!WeaponCurrent) return false;
	return WeaponCurrent->CanAttack();
}

void UWeaponManagerComponent::AttachInstanceToOwner(AWeapon* SpawnedWeapon, EItemCode InItemCode)
{
	if (!SpawnedWeapon) return;
	
	SpawnedWeapon->SetWeaponOwner(Cast<ACharacter>(Owner));

	SpawnedWeapon->AttachToComponent(
		Owner->GetMesh(),
		FAttachmentTransformRules::KeepWorldTransform,
		FName("root") // 임시로 root 소켓에 부착
	);

	SpawnedWeapon->WeaponActivate(false);
	WeaponInstances.Add(InItemCode, SpawnedWeapon);
}

void UWeaponManagerComponent::OnAreaAttack()
{
	if (!WeaponCurrent) return;
	WeaponCurrent->DamageToArea();
}
void UWeaponManagerComponent::EquipWeapon(EItemCode WeaponCode, int32 Count)
{
	if (!WeaponCurrent) return;
	WeaponCurrent->WeaponActivate(false);
	if (ShouldDropCurrentWeapon(WeaponCode))
	{
		DropCurrentWeapon();
	}
	EquipNewWeapon(WeaponCode, Count);
}
void UWeaponManagerComponent::EquipNewWeapon(EItemCode WeaponCode, int32 Count)
{
	WeaponCurrent = GetEquippedWeaponByItemCode(WeaponCode);
	WeaponCurrent->WeaponActivate(true);
	WeaponCurrent->SetUsedCountRemain(Count);
}
bool UWeaponManagerComponent::ShouldDropCurrentWeapon(EItemCode WeaponCode)
{
	return WeaponCode != EItemCode::EIC_Basic && WeaponCode != WeaponCurrent->GetWeaponID();
}
void UWeaponManagerComponent::DropWeapon(EItemCode WeaponCode)
{
	UPickupFactorySubsystem* subsystem = GetWorld()->GetSubsystem<UPickupFactorySubsystem>();
	if (subsystem)
	{
		subsystem->SpawnUsedWeaponByItemCode(
			WeaponCode,
			Cast<AActionCharacter>(Owner)->GetDropLocation()->GetComponentLocation(),
			Owner->GetActorRotation()
		);
	}
}
void UWeaponManagerComponent::DropCurrentWeapon()
{
	UPickupFactorySubsystem* subsystem = GetWorld()->GetSubsystem<UPickupFactorySubsystem>();
	if (subsystem)
	{
		int32 usedCount = WeaponCurrent->GetUsedCountRemain();

		APickup* pickup = subsystem->SpawnCurrentWeaponByItemCode(
			WeaponCurrent->GetWeaponID(),
			Cast<AActionCharacter>(Owner)->GetDropLocation()->GetComponentLocation(),
			Owner->GetActorRotation(),
			(Owner->GetActorForwardVector() + Owner->GetActorUpVector()) * 300.0f
		);

		if (!pickup) return;

		Cast<APickupWeapon>(pickup)->SetAttackCountRemain(usedCount);
	}
}
void UWeaponManagerComponent::ResetWeapon()
{
	if (!WeaponCurrent || WeaponCurrent->CanAttack()) return;

	DropWeapon(WeaponCurrent->GetWeaponID());
	EquipWeapon(EItemCode::EIC_Basic, 10);
}
void UWeaponManagerComponent::OnAttack()
{
	if (!WeaponCurrent) return;
	WeaponCurrent->OnAttack();
}



