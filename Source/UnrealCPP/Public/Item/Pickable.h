#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Pickable.generated.h"

UINTERFACE(MinimalAPI)
class UPickable : public UInterface
{
	GENERATED_BODY()
};

class UNREALCPP_API IPickable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pickup")
	void OnPickup(AActor* Target);
};
