#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/CommonEnums.h"
#include "ItemDataAsset.generated.h"

class APickupItem;

UCLASS()
class UNREALCPP_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item Data Asset")
	bool IsValid() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Type")
	EItemCode Code = EItemCode::EIC_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Name = FText::FromString(TEXT(""));
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Description = FText::FromString(TEXT(""));
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Cost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSubclassOf<APickupItem> Class;
};
