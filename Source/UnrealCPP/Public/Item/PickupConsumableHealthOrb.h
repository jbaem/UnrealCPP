#pragma once

#include "CoreMinimal.h"
#include "Item/PickupConsumable.h"
#include "PickupConsumableHealthOrb.generated.h"

UCLASS()
class UNREALCPP_API APickupConsumableHealthOrb : public APickupConsumable
{
	GENERATED_BODY()

protected:
	virtual void OnConsume() override;
};
