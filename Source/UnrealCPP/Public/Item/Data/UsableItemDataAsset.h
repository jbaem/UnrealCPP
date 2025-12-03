#pragma once

#include "CoreMinimal.h"
#include "Core/Interface/UsableItem.h"
#include "Item/Data/ItemDataAsset.h"

#include "UsableItemDataAsset.generated.h"

UCLASS()
class UNREALCPP_API UUsableItemDataAsset : public UItemDataAsset, public IUsableItem
{
	GENERATED_BODY()
	
public:
};
