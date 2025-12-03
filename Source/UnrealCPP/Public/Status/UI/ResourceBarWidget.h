#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ResourceBarWidget.generated.h"

UCLASS()
class UNREALCPP_API UResourceBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void RefreshWidget(float CurrentValue, float MaxValue);

	UFUNCTION(BlueprintCallable)
	void SetBarColorWithBackground(FLinearColor InColor);
	UFUNCTION(BlueprintCallable)
	void SetBarColor(FLinearColor InColor);
	UFUNCTION(BlueprintCallable)
	void SetBarBackgroundColor(FLinearColor InColor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Resource", meta = (BindWidget))
	TObjectPtr<class UProgressBar> Bar = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Resource", meta = (BindWidget))
	TObjectPtr<class UTextBlock> Current = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Resource", meta = (BindWidget))
	TObjectPtr<class UTextBlock> Max = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Color")
	FLinearColor FillColor = FLinearColor(0.0f, 1.0f, 0.0f);  //FLinearColor::Green;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Color")
	FLinearColor BackgroundColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.2f);

public:
#if WITH_EDITOR
	// Editor only 
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
