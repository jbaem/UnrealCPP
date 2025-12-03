#include "Item/Consumable/PickupConsumableMoney.h"

#include "Core/Interface/InventoryOwner.h"

void APickupConsumableMoney::OnConsume()
{
	Super::OnConsume();

	if(PickupTarget.IsValid() && PickupTarget->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddMoney(PickupTarget.Get(), MoneyAmount);
	}
}
