#include "Framework/UI/MainHUD.h"

#include "Blueprint/UserWidget.h"
#include "Framework/UI/MainHudWidget.h"
#include "Player/Base/ActionPlayerController.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	AddMainHudToViewPort();
}

void AMainHUD::AddMainHudToViewPort()
{
	if (!MainWidgetClass) return;
	
	MainWidget = CreateWidget<UMainHudWidget>(GetWorld(), MainWidgetClass);
	if (!MainWidget) return;

	MainWidget->AddToViewport();
	InitMainhud();
}

void AMainHUD::InitMainhud()
{
	if (AActionPlayerController* pc = Cast<AActionPlayerController>(GetOwningPlayerController()))
	{
		pc->InitializeMainHudWidget(MainWidget);
	}
}
