#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Core/Common/CommonEnums.h"
#include "Item/Data/ItemDataAsset.h"

#include "InventoryOwner.generated.h"

UINTERFACE(MinimalAPI)
class UInventoryOwner : public UInterface
{
	GENERATED_BODY()
};

class UNREALCPP_API IInventoryOwner
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void AddItem(UItemDataAsset* Itemdata, int32 Count);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void AddWeapon(EItemCode Code, int32 AttackCount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void AddMoney(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void UseMoney(int32 Amount);
};
