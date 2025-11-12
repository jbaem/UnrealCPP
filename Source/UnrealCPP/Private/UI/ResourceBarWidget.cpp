// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResourceBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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
