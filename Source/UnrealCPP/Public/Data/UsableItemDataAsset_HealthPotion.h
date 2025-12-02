#pragma once

#include "CoreMinimal.h"
#include "Data/UsableItemDataAsset.h"
#include "UsableItemDataAsset_HealthPotion.generated.h"

UCLASS()
class UNREALCPP_API UUsableItemDataAsset_HealthPotion : public UUsableItemDataAsset
{
	GENERATED_BODY()
public:
	virtual void UseItem_Implementation(AActor* InTarget) override;
};
