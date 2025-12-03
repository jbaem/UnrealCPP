#pragma once

#include "CoreMinimal.h"
#include "Item/Pickupable/Pickup.h"

#include "PickupItem.generated.h"

UCLASS()
class UNREALCPP_API APickupItem : public APickup
{
	GENERATED_BODY()
	
public:
	virtual void OnPickupComplete_Implementation() override;

protected:
	// using DataAsset
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup|Item")
	//EItemCode Code = EItemCode::EIC_None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup|Item")
	TObjectPtr<class UItemDataAsset> PickupItemData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup|Item")
	int32 Count = 3;
};
