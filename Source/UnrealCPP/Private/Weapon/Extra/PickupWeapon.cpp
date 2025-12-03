#include "Weapon/Extra/PickupWeapon.h"

#include "Core/Interface/InventoryOwner.h"

void APickupWeapon::OnPickupComplete_Implementation()
{
	if (PickupTarget.IsValid() && PickupTarget->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddWeapon(PickupTarget.Get(), ItemCode, AttackCountRemain);
	}
	Destroy();

	Super::OnPickupComplete_Implementation();
}
