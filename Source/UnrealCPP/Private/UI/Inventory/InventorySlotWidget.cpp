#include "UI/Inventory/InventorySlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/InventoryComponent.h"

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

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent); // Call the base implementation for other mouse buttons
}
