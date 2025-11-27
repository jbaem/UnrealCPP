#pragma once

#include "CoreMinimal.h"
#include "Data/ConsumableDataAsset.h"
#include "MoneyDataAsset.generated.h"

UCLASS()
class UNREALCPP_API UMoneyDataAsset : public UConsumableDataAsset
{
	GENERATED_BODY()
	
protected:
	virtual void Execute(AActor* Target) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Money")
	int32 MoneyAmount = 100;
};
