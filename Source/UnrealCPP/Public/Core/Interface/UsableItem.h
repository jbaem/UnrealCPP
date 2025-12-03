#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "UsableItem.generated.h"

UINTERFACE(MinimalAPI)
class UUsableItem : public UInterface
{
	GENERATED_BODY()
};

class UNREALCPP_API IUsableItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Usable")
	void UseItem(AActor* InTarget);
};
