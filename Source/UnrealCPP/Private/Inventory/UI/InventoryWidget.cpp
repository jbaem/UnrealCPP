#include "Inventory/UI/InventoryWidget.h"

#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Inventory/Base/InventoryComponent.h"
#include "Inventory/System/InventoryDragDropOperation.h"
#include "Inventory/UI/InventorySlotWidget.h"

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
				
				slotWidget->OnSlotRightClicked.Clear();
				// UFUNCTION 바인딩은 FName으로 함수 이름을 지정
				slotWidget->OnSlotRightClicked.BindUFunction(TargetInventory.Get(), FName("UseItem"));
				
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

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InMouseEvent, UDragDropOperation* InOperation)
{
	UInventoryDragDropOperation* DragDropOp = Cast<UInventoryDragDropOperation>(InOperation);
	if(DragDropOp)
	{
		UE_LOG(LogTemp, Log, TEXT("Dropped item: %s into InventoryWidget."), *DragDropOp->ItemData->Name.ToString());
		return true;
	}
	return false;
}
