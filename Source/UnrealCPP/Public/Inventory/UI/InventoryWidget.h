#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventoryWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryClosedRequested);

UCLASS()
class UNREALCPP_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	void InitializeInventoryWidget(class UInventoryComponent* InInventoryComponent);
	
	void RefreshInventoryWidget();
	void ClearInventoryWidget();

protected:
	UFUNCTION()
	void OnCloseClicked();

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InMouseEvent, UDragDropOperation* InOperation) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "UI|Inventory")
	FOnInventoryClosedRequested OnInventoryClosedRequest;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CloseButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> SlotGridPanel = nullptr;
private:
	UPROPERTY()
	TWeakObjectPtr<class UInventoryComponent> TargetInventory = nullptr;

	TArray<TObjectPtr<class UInventorySlotWidget>> SlotWidgets;
};
