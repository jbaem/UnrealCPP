#include "Data/ItemDataAsset.h"

#include "Item/PickupItem.h"

bool UItemDataAsset::IsValid() const
{
	return Class != nullptr;
}

