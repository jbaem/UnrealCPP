#pragma once

#include "CoreMinimal.h"
#include "Core/Common/CommonEnums.h"

#include "CommonStructs.generated.h"

USTRUCT(BlueprintType)
struct FTestStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TestStruct")
	int32 DataA = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TestStruct")
	float DataB = 10.0f;
};

USTRUCT(BlueprintType)
struct FDropItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Item")
	TSubclassOf<class APickup> DropItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Item")
	float DropItemRate = 0.0f;
};
