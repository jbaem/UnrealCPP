#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "EnemyDamageWidget.generated.h"

UCLASS()
class UNREALCPP_API UEnemyDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetDamageText(float DamageAmount);

	void PlayPopAnimation();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor FontColor = FLinearColor::Yellow;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidgetAnim))
	//TObjectPtr<UWidgetAnimation> PopAnim = nullptr;
};
