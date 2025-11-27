#pragma once

#include "CoreMinimal.h"
#include "Item/Pickup.h"
#include "Data/ConsumableDataAsset.h"
#include "PickupConsumable.generated.h"

UCLASS()
class UNREALCPP_API APickupConsumable : public APickup
{
	GENERATED_BODY()
	
protected:
	virtual void OnConsume();
	virtual void OnPickupComplete_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup|Consumable")
	TObjectPtr<UConsumableDataAsset> DataAsset = nullptr;
};
