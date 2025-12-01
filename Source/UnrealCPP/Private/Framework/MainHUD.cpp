#include "Framework/MainHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/MainHudWidget.h"
#include "Player/ActionPlayerController.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UMainHudWidget>(GetWorld(), MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();

			AActionPlayerController* pc = Cast<AActionPlayerController>(GetOwningPlayerController());
			if(pc)
			{
				pc->InitializeMainHudWidget(MainWidget);
			}
		}
	}
}
