#include "Inventory/Base/InventoryComponent.h"

#include "Core/Interface/UsableItem.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Slots.SetNum(InventorySize);
}

/// <summary>
/// 1. 같은 종류 아이템은 같은 칸에 들어감
/// 2. 최대 수량이 넘을 경우 남은 수량은 빈 칸에 들어감
/// 3. 빈 칸이 없을 경우 남은 수량을 반환함
/// <returns>남은 수량</returns>
int32 UInventoryComponent::AddItem(UItemDataAsset* InDataAsset, int32 InQuantity)
{
	int32 QuantityRemain = InQuantity;
	if (!InDataAsset || QuantityRemain < 1)
	{
		return QuantityRemain;
	}

	int32 StartIndex = 0;
	while (QuantityRemain > 0)
	{
		int32 FoundIndex = FindSlotIndexByItem(InDataAsset, StartIndex);
		if (FoundIndex == UInventoryComponent::InventoryFailedIndex)
		{
			break;
		}
		FInvenSlot& Slot = Slots[FoundIndex];
		int32 QuantityAvailable = Slot.ItemDataAsset->QuantityMax - Slot.GetQuantity();
		int32 QuantityAdded = FMath::Min(QuantityAvailable, QuantityRemain);
		if (QuantityAdded > 0)
		{
			SetItemAt(FoundIndex, InDataAsset, Slot.GetQuantity() + QuantityAdded);
			QuantityRemain -= QuantityAdded;
		}
		StartIndex = FoundIndex + 1;
	}
	
	StartIndex = 0;
	while (QuantityRemain > 0)
	{
		int32 EmptyIndex = FindSlotIndexIsEmpty(StartIndex);
		if (EmptyIndex == UInventoryComponent::InventoryFailedIndex)
		{
			break;
		}
		FInvenSlot& Slot = Slots[EmptyIndex];
		int32 QuantityAdded = FMath::Min(Slot.ItemDataAsset->QuantityMax, QuantityRemain);
		if (QuantityAdded > 0)
		{
			SetItemAt(EmptyIndex, InDataAsset, QuantityAdded);
			QuantityRemain -= QuantityAdded;
		}
		StartIndex = EmptyIndex + 1;
	}
	return QuantityRemain;
}

void UInventoryComponent::SetItemAt(int32 Index, UItemDataAsset* InDataAsset, int32 InQuantity)
{
	if(!IsValidIndex(Index)) return;

	FInvenSlot& Slot = Slots[Index];
	
	Slot.ItemDataAsset = InDataAsset;
	Slot.SetQuantity(InQuantity);
}

void UInventoryComponent::UpdateQuantityAt(int32 Index, int32 DeltaQuantity)
{
	if (!IsValidIndex(Index)) return;

	FInvenSlot& Slot = Slots[Index];
	if (Slot.IsEmpty()) return;

	int32 NewQuantity = Slot.GetQuantity() + DeltaQuantity;
	SetItemAt(Index, Slot.ItemDataAsset, NewQuantity);
}

void UInventoryComponent::ClearSlotAt(int32 Index)
{
	if (!IsValidIndex(Index)) return;

	FInvenSlot& Slot = Slots[Index];
	Slot.Clear();
}

FInvenSlot* UInventoryComponent::GetSlotDataAt(int32 Index)
{
	check(IsValidIndex(Index));
	return Slots[Index].IsEmpty() ? nullptr : &Slots[Index];
}

void UInventoryComponent::UseItem(int32 InIndex)
{
	UE_LOG(LogTemp, Log, TEXT("UseItem called at index: %d"), InIndex);
	FInvenSlot* slot = GetSlotDataAt(InIndex);
	if (slot->ItemDataAsset->Implements<UUsableItem>())
	{
		IUsableItem::Execute_UseItem(slot->ItemDataAsset, this->GetOwner());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item at index %d does not implement IUsableItem interface."), InIndex);
	}
}

int32 UInventoryComponent::FindSlotIndexByItem(UItemDataAsset* InDataAsset, int32 StartIndex)
{
	int32 Size = Slots.Num();
	for(int32 i = StartIndex; i < Size; ++i)
	{
		FInvenSlot& Slot = Slots[i];
		if (Slot.IsFull() || Slot.IsEmpty()) continue;
		
		if (Slot.ItemDataAsset == InDataAsset)
		{
			return i;
		}
	}
	return UInventoryComponent::InventoryFailedIndex;
}

int32 UInventoryComponent::FindSlotIndexIsEmpty(int32 StartIndex)
{
	int32 Size = Slots.Num();
	for (int32 i = StartIndex; i < Size; ++i)
	{
		if (Slots[i].IsEmpty())
		{
			return i;
		}
	}
	return UInventoryComponent::InventoryFailedIndex;
}
