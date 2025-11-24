#include "Widget/DamageWidget.h"

#include "Components/TextBlock.h"

void UDamageWidget::SetDamagetText(float Damage)
{
	if(DamageText)
	{
		//FMath::RoundToFloat(Damage);
		DamageText->SetText(FText::AsNumber(static_cast<int32>(Damage)));
	}
}

void UDamageWidget::PlayPopupAnimation()
{
	if(PopupAnimation)
	{
		PlayAnimation(PopupAnimation);
	}
}
