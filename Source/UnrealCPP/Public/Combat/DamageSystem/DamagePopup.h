#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Combat/UI/DamageWidget.h"

#include "DamagePopup.generated.h"

UCLASS(Blueprintable)
class UNREALCPP_API ADamagePopup : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamagePopup();
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ActivatePopup(float InDamage = 0.0f);
	UFUNCTION(BlueprintCallable)
	void DeactivatePopup();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data|Components")
	TObjectPtr<class UWidgetComponent> WidgetComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Life Time", meta = (ClampMin = "1.0"))
	float LifeTime = 1.5f;

private:
	void InitWidgetComponent();
	void CreateWidgetInstance();
	void ActivatePopupInstance(float InDamage);

	TObjectPtr<class UDamageWidget> WidgetInstance = nullptr;
	FTimerHandle LifeTimerHandle;
};
