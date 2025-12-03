#include "Item/Consumable/PickupConsumable.h"

void APickupConsumable::OnConsume()
{
	if (DataAsset)
	{
		DataAsset->Execute(PickupTarget.Get());
	}
}

void APickupConsumable::OnPickupComplete_Implementation()
{
	OnConsume();

	Super::OnPickupComplete_Implementation();
}
