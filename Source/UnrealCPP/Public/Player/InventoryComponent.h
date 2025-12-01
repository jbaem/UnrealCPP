#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ItemDataAsset.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInvenSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot")
	TObjectPtr<UItemDataAsset> ItemDataAsset = nullptr;
public:
	bool IsEmpty() const { return ItemDataAsset == nullptr || Quantity < 1; }
	bool IsFull() const { return ItemDataAsset && Quantity >= ItemDataAsset->QuantityMax; }
	void Clear() { ItemDataAsset = nullptr; Quantity = 0; }
	int32 GetQuantity() const { return Quantity; }
	void SetQuantity(int32 NewQuantity) 
	{ 
		if (IsEmpty() || NewQuantity < 1) 
		{
			Clear();
			return;
		}
		Quantity = FMath::Clamp(NewQuantity, 0, ItemDataAsset->QuantityMax); 
	}
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Slot", meta = (ClampMin = "0"))
	int32 Quantity = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	static const int32 InventoryFailedIndex = -1;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 AddItem(UItemDataAsset* InDataAsset, int32 InQuantity);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetItemAt(int32 Index, UItemDataAsset* InDataAsset, int32 Quantity);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateQuantityAt(int32 Index, int32 DeltaQuantity = -1);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearSlotAt(int32 Index);

	FInvenSlot* GetSlotDataAt(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	inline bool IsValidIndex(int32 Index) const { return Index < InventorySize && Index >= 0; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	inline int32 GetInventorySize() const { return InventorySize; }

private:
	int32 FindSlotIndexByItem(UItemDataAsset* InDataAsset, int32 StartIndex = 0);
	int32 FindSlotIndexIsEmpty(int32 StartIndex = 0);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 InventorySize = 4;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInvenSlot> Slots;
};
