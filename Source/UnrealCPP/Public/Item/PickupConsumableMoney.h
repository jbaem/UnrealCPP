#pragma once

#include "CoreMinimal.h"
#include "Item/PickupConsumable.h"

#include "PickupConsumableMoney.generated.h"

UCLASS()
class UNREALCPP_API APickupConsumableMoney : public APickupConsumable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickupConsumableMoney")
	int32 MoneyAmount = 100;

	virtual void OnConsume() override;
};
