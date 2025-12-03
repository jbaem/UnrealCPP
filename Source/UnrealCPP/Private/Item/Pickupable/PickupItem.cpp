#include "Item/Pickupable/PickupItem.h"

#include "Core/Interface/InventoryOwner.h"

void APickupItem::OnPickupComplete_Implementation()
{
	if(PickupTarget.IsValid() && PickupTarget->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddItem(PickupTarget.Get(), PickupItemData, Count);
	}
	Super::OnPickupComplete_Implementation();
}