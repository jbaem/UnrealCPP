#include "Framework/UI/MainHudWidget.h"

#include "Player/Base/ActionCharacter.h"
#include "Status/Base/ResourceComponent.h"
#include "Status/UI/ResourceBarWidget.h"

void UMainHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitResourceBar();
	InitInventory();
}

void UMainHudWidget::OpenInventory()
{
	Inventory->RefreshInventoryWidget();
	Inventory->SetVisibility(ESlateVisibility::Visible);
	InventoryState = EInventoryState::EIS_Open;
}

void UMainHudWidget::CloseInventory()
{
	Inventory->SetVisibility(ESlateVisibility::Hidden);
	InventoryState = EInventoryState::EIS_Close;
}

void UMainHudWidget::InitResourceBar()
{
	AActionCharacter* Player = Cast<AActionCharacter>(GetOwningPlayerPawn());
	if (!Player) return;

	BindResourceComponentToBar(Player);
	InitResourceBarColor();
}

void UMainHudWidget::AddToInventoryCloseDelegate(const FScriptDelegate& Delegate)
{
	if (!Inventory) return;
	Inventory->OnInventoryClosedRequest.Add(Delegate);
}

void UMainHudWidget::InitInventory()
{
	CloseInventory();
}

void UMainHudWidget::BindResourceComponentToBar(AActionCharacter* Player)
{
	UResourceComponent* ResourceComponent = Player->GetResourceComponent();
	if (!ResourceComponent) return;
	// binding 하는 함수가 다르다 // multicast delegate vs dynamic multicast delegate
	ResourceComponent->OnHealthChanged.AddUObject(HealthBar.Get(), &UResourceBarWidget::RefreshWidget);
	ResourceComponent->OnStaminaChanged.AddDynamic(StaminaBar.Get(), &UResourceBarWidget::RefreshWidget);
}

void UMainHudWidget::InitResourceBarColor()
{
	HealthBar->SetBarColorWithBackground(FLinearColor::Green);
	StaminaBar->SetBarColorWithBackground(FLinearColor::Yellow);
}

