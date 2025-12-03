#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Core/Common/CommonEnums.h"

#include "DataTableRows.generated.h"

USTRUCT(BlueprintType)
struct UNREALCPP_API FDropItemData_TableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemCode ItemCode = EItemCode::EIC_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1"))
	float DropItemRate = 1.0f;	
};

USTRUCT(BlueprintType)
struct UNREALCPP_API FPickupData_TableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemCode ItemCode = EItemCode::EIC_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1"))
	TSubclassOf<class APickup> PickupClass;
};
