#include "Framework/UI/MainHUD.h"

#include "Blueprint/UserWidget.h"
#include "Framework/UI/MainHudWidget.h"
#include "Player/Base/ActionPlayerController.h"

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
