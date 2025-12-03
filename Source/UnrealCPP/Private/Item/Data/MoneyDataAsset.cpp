#include "Item/Data/MoneyDataAsset.h"

#include "Core/Interface/InventoryOwner.h"

void UMoneyDataAsset::Execute(AActor* Target) const
{
	Super::Execute(Target);

	if (Target && Target->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddMoney(Target, MoneyAmount);
	}
}
