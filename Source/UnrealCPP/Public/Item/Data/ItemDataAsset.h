#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/Common/CommonEnums.h"

#include "ItemDataAsset.generated.h"

class APickupItem;

UCLASS()
class UNREALCPP_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemCode Code = EItemCode::EIC_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FText Name = FText::FromString(TEXT("아이템"));
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FText Description = FText::FromString(TEXT("아이템 설명"));
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (ClampMin = "0"))
	int32 Cost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (ClampMin = "0"))
	int32 QuantityMax = 10;

	// using ItemCode 
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	//TSubclassOf<APickupItem> Class;
};
