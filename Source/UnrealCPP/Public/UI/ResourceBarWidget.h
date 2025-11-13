// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ResourceBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API UResourceBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void RefreshWidget(float CurrentValue, float MaxValue);

	// Editor only 
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable)
	void SetBarColorWithBackground(FLinearColor InColor);

	UFUNCTION(BlueprintCallable)
	void SetBarColor(FLinearColor InColor);

	UFUNCTION(BlueprintCallable)
	void SetBarBackgroundColor(FLinearColor InColor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UProgressBar> Bar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UTextBlock> Current = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UTextBlock> Max = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	FLinearColor FillColor = FLinearColor(0.0f, 1.0f, 0.0f);  //FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Color")
	FLinearColor BackgroundColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.2f);
};
