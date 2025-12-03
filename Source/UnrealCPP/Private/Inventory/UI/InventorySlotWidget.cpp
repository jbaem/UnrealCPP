#include "Inventory/UI/InventorySlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Base/InventoryComponent.h"
#include "Inventory/System/InventoryDragDropOperation.h"

void UInventorySlotWidget::InitializeSlot(int32 InIndex, FInvenSlot* InSlotData)
{
	Index = InIndex;
	SlotData = InSlotData;	

	RefreshSlot();
}

void UInventorySlotWidget::RefreshSlot() const
{
	if (SlotData && !SlotData->IsEmpty())
	{
		ItemIconImage->SetBrushFromTexture(SlotData->ItemDataAsset->Icon);
		CountText->SetText(FText::AsNumber(SlotData->GetQuantity()));
		MaxCountText->SetText(FText::AsNumber(SlotData->ItemDataAsset->QuantityMax));

		ItemIconImage->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SeperateText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MaxCountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		ClearSlotWidget();
	}
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UInventoryDragDropOperation* DragDropOp = NewObject<UInventoryDragDropOperation>();
	DragDropOp->Index = Index;
	DragDropOp->ItemData = SlotData->ItemDataAsset;

	OutOperation = DragDropOp;
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InMouseEvent, UDragDropOperation* InOperation)
{
	UInventoryDragDropOperation* DragDropOp = Cast<UInventoryDragDropOperation>(InOperation);
	if(DragDropOp)
	{
		UE_LOG(LogTemp, Log, TEXT("Dropped item: %s into slot index: %d"), *DragDropOp->ItemData->Name.ToString(), Index);
		return true;
	}
	return false;
	//return Super::NativeOnDrop(InGeometry, InMouseEvent, InOperation);
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InMouseEvent, UDragDropOperation* InOperation)
{
	UInventoryDragDropOperation* DragDropOp = Cast<UInventoryDragDropOperation>(InOperation);
	if (DragDropOp)
	{
		UE_LOG(LogTemp, Log, TEXT("Drag operation cancelled for item: %s"), *DragDropOp->ItemData->Name.ToString());
	}

	Super::NativeOnDragCancelled(InMouseEvent, InOperation);
}

void UInventorySlotWidget::ClearSlotWidget() const
{
	ItemIconImage->SetBrushFromTexture(nullptr);
	ItemIconImage->SetBrushTintColor(FLinearColor::Transparent);
	
	CountText->SetVisibility(ESlateVisibility::Hidden);
	SeperateText->SetVisibility(ESlateVisibility::Hidden);
	MaxCountText->SetVisibility(ESlateVisibility::Hidden);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		if (SlotData && SlotData->ItemDataAsset)
		{
			UE_LOG(LogTemp, Log, TEXT("Right mouse button clicked on InventorySlotWidget with item: %s"), *SlotData->ItemDataAsset->Name.ToString());
			OnSlotRightClicked.ExecuteIfBound(Index);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Right mouse button clicked on empty InventorySlotWidget"));
		}
		return FReply::Handled(); // Indicate that the event was handled
	}
	else if(InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Log, TEXT("Left mouse button clicked on InventorySlotWidget"));
		if(SlotData && !SlotData->IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("Starting drag operation for item: %s"), *SlotData->ItemDataAsset->Name.ToString());
			//return FReply::Handled(); // Indicate that the event was handled
			return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton); // Indicate that the event was handled
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Cannot start drag operation: Slot is empty"));
		}
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent); // Call the base implementation for other mouse buttons
}
