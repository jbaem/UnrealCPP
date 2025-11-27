#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ConsumableDataAsset.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UNREALCPP_API UConsumableDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Pickup|Consumable")
	virtual void Execute(AActor* Target) const {};

};
