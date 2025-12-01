#include "UI/Inventory/InventoryWidget.h"

#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "UI/Inventory/InventorySlotWidget.h"
#include "Player/InventoryComponent.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseClicked);
	}
}

void UInventoryWidget::InitializeInventoryWidget(UInventoryComponent* InInventoryComponent)
{
	if (InInventoryComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("InventoryWidget initialized with InventoryComponent having slots."));
		TargetInventory = InInventoryComponent;
		if(TargetInventory.IsValid())
		{
			if (SlotGridPanel->GetChildrenCount() != TargetInventory->GetInventorySize())
			{
				UE_LOG(LogTemp, Warning, TEXT("SlotGridPanel children count (%d) does not match TargetInventory size (%d)."), SlotGridPanel->GetChildrenCount(), TargetInventory->GetInventorySize());
				return;
			}

			int32 size = FMath::Min(SlotGridPanel->GetChildrenCount(), TargetInventory->GetInventorySize());
			for (int i = 0; i < size; ++i)
			{
				FInvenSlot* slotData = TargetInventory->GetSlotDataAt(i);
				UInventorySlotWidget* slotWidget = Cast<UInventorySlotWidget>(SlotGridPanel->GetChildAt(i));
				slotWidget->InitializeSlot(i, slotData);
				SlotWidgets.Add(slotWidget);
			}
		}
	}
}

void UInventoryWidget::RefreshInventoryWidget()
{
	for(const UInventorySlotWidget* slotWidget : SlotWidgets)
	{
		if(slotWidget)
		{
			slotWidget->RefreshSlot();
		}
	}
}

void UInventoryWidget::ClearInventoryWidget()
{
	TargetInventory = nullptr;
}

void UInventoryWidget::OnCloseClicked()
{
	OnInventoryClosedRequest.Broadcast();
}
