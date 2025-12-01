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

void UInventorySlotWidget::RefreshSlot()
{
	if (SlotData)
	{
		ItemIconImage->SetBrushFromTexture(SlotData->ItemDataAsset->Icon);
		ItemIconImage->SetBrushTintColor(FLinearColor::White);

		CountText->SetText(FText::AsNumber(SlotData->GetQuantity()));
		MaxCountText->SetText(FText::AsNumber(SlotData->ItemDataAsset->QuantityMax));

		CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SeperateText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MaxCountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		ClearSlotWidget();
	}
}

void UInventorySlotWidget::ClearSlotWidget()
{
	ItemIconImage->SetBrushFromTexture(nullptr);
	ItemIconImage->SetBrushTintColor(FLinearColor::Transparent);
	
	CountText->SetVisibility(ESlateVisibility::Hidden);
	SeperateText->SetVisibility(ESlateVisibility::Hidden);
	MaxCountText->SetVisibility(ESlateVisibility::Hidden);
}
