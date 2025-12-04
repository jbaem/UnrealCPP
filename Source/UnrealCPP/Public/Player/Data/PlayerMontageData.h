#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "PlayerMontageData.generated.h"

UCLASS()
class UNREALCPP_API UPlayerMontageData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UAnimMontage> Roll = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> Attack = nullptr;
};
