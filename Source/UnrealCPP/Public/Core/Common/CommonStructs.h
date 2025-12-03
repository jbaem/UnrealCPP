#pragma once

#include "CoreMinimal.h"
#include "Core/Common/CommonEnums.h"

#include "CommonStructs.generated.h"

USTRUCT(BlueprintType)
struct FDropItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Item")
	TSubclassOf<class APickup> DropItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Item")
	float DropItemRate = 0.0f;
};
