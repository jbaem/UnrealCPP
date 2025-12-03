#pragma once

#include "CoreMinimal.h"
#include "Item/Data/ConsumableDataAsset.h"

#include "HealthOrbDataAsset.generated.h"

UCLASS()
class UNREALCPP_API UHealthOrbDataAsset : public UConsumableDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float HealthAmount = 25.0f;

	virtual void Execute(AActor* Target) const override;
};
