#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerData.generated.h"

UCLASS()
class UNREALCPP_API UPlayerData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Movement|Walk")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Movement|Sprint")
	float SprintSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Movement|Sprint")
	float SprintStaminaCost = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Movement|Roll")
	float RollStaminaCost = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Combat|Attack")
	float AttackStaminaCost = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Combat|Attack")
	float Attack2StaminaCost = 15.0f;
};
