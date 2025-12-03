#include "Status/UI/ResourceBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UResourceBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetBarColor(FillColor);
	SetBarBackgroundColor(BackgroundColor);
}

void UResourceBarWidget::RefreshWidget(float CurrentValue, float MaxValue)
{
	if(MaxValue <= 0.0f)
	{
		Bar->SetPercent(0.0f);
		Current->SetText(FText::FromString("0"));
		Max->SetText(FText::FromString("0"));
		return;
	}

	Bar->SetPercent(CurrentValue / MaxValue);
	Current->SetText(FText::AsNumber(FMath::FloorToInt(CurrentValue)));
	Max->SetText(FText::AsNumber(FMath::FloorToInt(MaxValue)));	
}

void UResourceBarWidget::SetBarColorWithBackground(FLinearColor InColor)
{
	SetBarColor(InColor);
	SetBarBackgroundColor(InColor.CopyWithNewOpacity(0.2));
}

void UResourceBarWidget::SetBarColor(FLinearColor InColor)
{
	if(Bar)
	{
		Bar->SetFillColorAndOpacity(InColor);
	}
}

void UResourceBarWidget::SetBarBackgroundColor(FLinearColor InColor)
{
	FProgressBarStyle style = Bar->GetWidgetStyle();
	style.BackgroundImage.TintColor = InColor;
	Bar->SetWidgetStyle(style);
}

#if WITH_EDITOR
// UObject 멤버 변수에 변화가 있을 때마다 호출되는 함수
void UResourceBarWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName propertyName =
		PropertyChangedEvent.Property != nullptr ?
		PropertyChangedEvent.Property->GetFName() :
		NAME_None;

	if (propertyName == GET_MEMBER_NAME_CHECKED(UResourceBarWidget, FillColor))
	{
		BackgroundColor = FillColor;
		BackgroundColor.A = 0.2f;
	}
}
#endif