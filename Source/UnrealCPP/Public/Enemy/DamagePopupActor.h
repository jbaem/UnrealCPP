#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widget/DamageWidget.h"

#include "DamagePopupActor.generated.h"

UCLASS(Blueprintable)
class UNREALCPP_API ADamagePopupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamagePopupActor();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void PopupActivate(float Damage = 0.0f);
	UFUNCTION(BlueprintCallable)
	void PopupDeactivate();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UWidgetComponent> DamageWidgetComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ClampMin = "1.0"))
	float LifeTime = 1.5f;
	
private:
	TObjectPtr<class UDamageWidget> DamageWidgetInstance = nullptr;

	FTimerHandle LifeTimerHandle;
};
