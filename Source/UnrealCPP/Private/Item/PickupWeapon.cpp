#include "Item/PickupWeapon.h"

#include "Player/InventoryOwner.h"

void APickupWeapon::OnPickupComplete_Implementation()
{
	if (PickupTarget.IsValid() && PickupTarget->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddWeapon(PickupTarget.Get(), ItemCode, AttackCountRemain);
	}
	Destroy();

	Super::OnPickupComplete_Implementation();
}
