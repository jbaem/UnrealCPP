#include "UI/Inventory/InventoryWidget.h"

#include "Components/Button.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnCloseClicked);
	}
}

void UInventoryWidget::OnCloseClicked()
{
	OnInventoryClosedRequest.Broadcast();
}
