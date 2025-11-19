#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Common/CommonEnums.h"

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
	void AddItem(EItemCode Code, int32 Count);
};
