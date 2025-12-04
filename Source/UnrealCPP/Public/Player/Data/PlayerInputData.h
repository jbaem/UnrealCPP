#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"

#include "PlayerInputData.generated.h"

UCLASS()
class UNREALCPP_API UPlayerInputData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement")
	TObjectPtr<UInputAction> Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement")
	TObjectPtr<UInputAction> Sprint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement")
	TObjectPtr<UInputAction> Roll = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Combat")
	TObjectPtr<UInputAction> Attack = nullptr;
};
