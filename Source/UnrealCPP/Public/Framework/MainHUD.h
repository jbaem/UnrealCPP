#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "MainHUD.generated.h"

UCLASS()
class UNREALCPP_API AMainHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

protected:
	// TSubclassOf: that can hold a reference to any UUserWidget class or subclass
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> MainWidgetClass = nullptr;
};
