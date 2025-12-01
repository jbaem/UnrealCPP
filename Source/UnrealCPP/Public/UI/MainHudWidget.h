#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/InventoryWidget.h"
#include "MainHudWidget.generated.h"

UENUM(BlueprintType)
enum class EInventoryState : uint8
{
	EIS_Close		UMETA(DisplayName = "Close"),
	EIS_Open		UMETA(DisplayName = "Open"),
};

UCLASS()
class UNREALCPP_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void OpenInventory();
	UFUNCTION(BlueprintCallable, Category = "UI|Inventory")
	void CloseInventory();

	void AddToInventoryCloseDelegate(const FScriptDelegate& Delegate)
	{
		if(!Inventory) return;
		Inventory->OnInventoryClosedRequest.Add(Delegate);
	}

	inline EInventoryState GetInventoryState() const { return InventoryState; }

private:
	void InitResourceBar();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> HealthBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UResourceBarWidget> StaminaBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
	TObjectPtr<class UInventoryWidget> Inventory = nullptr;
private:
	EInventoryState InventoryState = EInventoryState::EIS_Close;
};
