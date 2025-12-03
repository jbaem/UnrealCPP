#include "Combat/UI/DamageWidget.h"

#include "Components/TextBlock.h"

void UDamageWidget::SetDamagetText(float Damage)
{
	if (!DamageText) return;
	//FMath::RoundToFloat(Damage);
	DamageText->SetText(FText::AsNumber(static_cast<int32>(Damage)));
}

void UDamageWidget::PlayPopupAnimation()
{
	if (!PopupAnimation) return;
	PlayAnimation(PopupAnimation);
}
