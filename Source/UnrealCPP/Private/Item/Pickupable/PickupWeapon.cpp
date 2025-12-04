#include "Item/Pickupable/PickupWeapon.h"

#include "Core/Interface/InventoryOwner.h"
#include "Weapon/Manager/WeaponManagerComponent.h"

void APickupWeapon::OnPickupComplete_Implementation()
{
	if (!PickupTarget.IsValid()) return;

	if (UWeaponManagerComponent* WeaponManagerComponent = PickupTarget->GetComponentByClass<UWeaponManagerComponent>())
	{
		WeaponManagerComponent->EquipWeapon(ItemCode, AttackCountRemain);
	}
	Destroy();

	Super::OnPickupComplete_Implementation();
}
