#include "Inventory/UI/TemporarySlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTemporarySlotWidget::SetItemIconImage(UTexture2D* InTexture)
{
	ItemIconImage->SetBrushFromTexture(InTexture);
}

void UTemporarySlotWidget::SetCountText(int32 InCount)
{
	CountText->SetText(FText::AsNumber(InCount));
}
