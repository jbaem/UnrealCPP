#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InventorySlotWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSlotClicked, int32, InSlotIndex);

UCLASS()
class UNREALCPP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeSlot(int32 InIndex, struct FInvenSlot* InSlotData);
	void RefreshSlot() const;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InMouseEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InMouseEvent, UDragDropOperation* InOperation) override;

protected:
	void ClearSlotWidget() const;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	FOnSlotClicked OnSlotRightClicked;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UTextBlock> CountText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UTextBlock> SeperateText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|InventorySlot", meta = (BindWidget))
	TObjectPtr<class UTextBlock> MaxCountText = nullptr;

private:
	int32 Index = -1;
	struct FInvenSlot* SlotData = nullptr;
};
