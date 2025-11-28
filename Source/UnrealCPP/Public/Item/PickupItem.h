#pragma once

#include "CoreMinimal.h"
#include "Item/Pickup.h"
#include "PickupItem.generated.h"

UCLASS()
class UNREALCPP_API APickupItem : public APickup
{
	GENERATED_BODY()
	
public:
	virtual void OnPickupComplete_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EItemCode Code = EItemCode::EIC_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 Count = 3;
};
