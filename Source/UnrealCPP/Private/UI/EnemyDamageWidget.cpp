#include "UI/EnemyDamageWidget.h"

#include "Components/TextBlock.h"

void UEnemyDamageWidget::NativePreConstruct()
{

}

void UEnemyDamageWidget::SetDamageText(float DamageAmount)
{
	if (DamageText)
	{
		DamageText->SetColorAndOpacity(FSlateColor(FontColor));
		
		FString DamageString = FString::Printf(TEXT("%.1f"), DamageAmount);
		DamageText->SetText(FText::FromString(DamageString));
	}
}

void UEnemyDamageWidget::PlayPopAnimation()
{
	//if (PopAnim)
	//{
	//	PlayAnimation(PopAnim);
	//}
}
