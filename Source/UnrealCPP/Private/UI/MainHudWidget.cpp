#include "UI/MainHudWidget.h"

#include "Player/ActionCharacter.h"
#include "Player/ResourceComponent.h"
#include "UI/ResourceBarWidget.h"
#include "UI/Inventory/InventoryWidget.h"

void UMainHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitResourceBar();

	if (AActionCharacter* player = Cast<AActionCharacter>(GetOwningPlayerPawn()))
	{
		if(UInventoryComponent* InventoryComponent = player->GetInventoryComponent())
		{
			Inventory->OnInventoryClosedRequest.AddDynamic(this, &UMainHudWidget::CloseInventory);
		}
	}
	CloseInventory();

}

void UMainHudWidget::OpenInventory()
{
	Inventory->SetVisibility(ESlateVisibility::Visible);
}

void UMainHudWidget::CloseInventory()
{
	Inventory->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHudWidget::InitResourceBar()
{
	if (AActionCharacter* player = Cast<AActionCharacter>(GetOwningPlayerPawn()))
	{
		if (UResourceComponent* resourceComponent = player->GetResourceComponent())
		{
			// binding 하는 함수가 다르다 // multicast delegate vs dynamic multicast delegate
			resourceComponent->OnHealthChanged.AddUObject(HealthBar.Get(), &UResourceBarWidget::RefreshWidget);
			resourceComponent->OnStaminaChanged.AddDynamic(StaminaBar.Get(), &UResourceBarWidget::RefreshWidget);

			HealthBar->SetBarColorWithBackground(FLinearColor::Green);
			StaminaBar->SetBarColorWithBackground(FLinearColor::Yellow);
		}
	}
}
