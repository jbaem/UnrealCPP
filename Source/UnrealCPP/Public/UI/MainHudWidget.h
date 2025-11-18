#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainHudWidget.generated.h"

UCLASS()
class UNREALCPP_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> HealthBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> StaminaBar = nullptr;
};
