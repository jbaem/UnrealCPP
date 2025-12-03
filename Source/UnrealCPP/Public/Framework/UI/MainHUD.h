#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "MainHUD.generated.h"

class UMainHudWidget;

UCLASS()
class UNREALCPP_API AMainHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	inline UMainHudWidget* GetMainWidget() const { return MainWidget; }

protected:
	// TSubclassOf: that can hold a reference to any UUserWidget class or subclass
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UMainHudWidget> MainWidgetClass = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UMainHudWidget> MainWidget = nullptr;

private:
	void AddMainHudToViewPort();
	void InitMainhud();
};
