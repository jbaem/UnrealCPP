#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDataAsset.h"
#include "Item/UsableItem.h"
#include "UsableItemDataAsset.generated.h"

UCLASS()
class UNREALCPP_API UUsableItemDataAsset : public UItemDataAsset, public IUsableItem
{
	GENERATED_BODY()
	
public:
};
