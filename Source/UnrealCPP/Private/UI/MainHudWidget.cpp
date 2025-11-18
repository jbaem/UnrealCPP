#include "UI/MainHudWidget.h"

#include "Player/ActionCharacter.h"
#include "Player/ResourceComponent.h"
#include "UI/ResourceBarWidget.h"

void UMainHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AActionCharacter* player = Cast<AActionCharacter>(GetOwningPlayerPawn());
	if (player)
	{
		UResourceComponent* resourceComponent = player->GetResourceComponent();
		if (resourceComponent)
		{
			// binding 하는 함수가 다르다 // multicast delegate vs dynamic multicast delegate
			resourceComponent->OnHealthChanged.AddUObject(HealthBar.Get(), &UResourceBarWidget::RefreshWidget);
			resourceComponent->OnStaminaChanged.AddDynamic(StaminaBar.Get(), &UResourceBarWidget::RefreshWidget);
		
			HealthBar->SetBarColorWithBackground(FLinearColor::Green); 
			StaminaBar->SetBarColorWithBackground(FLinearColor::Yellow);
		}
	}
}
