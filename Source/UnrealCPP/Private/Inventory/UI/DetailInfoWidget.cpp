#include "Inventory/UI/DetailInfoWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/Data/ItemDataAsset.h"

void UDetailInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
}

void UDetailInfoWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateLocation();
}

void UDetailInfoWidget::Open(UItemDataAsset * InItemData)
{
	SetInfo(InItemData);
	UpdateLocation();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UDetailInfoWidget::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UDetailInfoWidget::SetInfo(UItemDataAsset * InItemData)
{
	if (!InItemData) return;
	//ItemNameText->SetText(FText::FromName(InItemData->Name));
	//ItemPriceText->SetText(FText::AsNumber(InItemData->Cost));	
	//ItemIconImage->SetBrushFromTexture(InItemData->Icon);
}

void UDetailInfoWidget::UpdateLocation()
{
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	CanvasPanelSlot->SetPosition(MousePosition - ParentPosition);
}
