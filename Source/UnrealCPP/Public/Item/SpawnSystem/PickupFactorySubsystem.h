#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/Common/CommonEnums.h"

#include "PickupFactorySubsystem.generated.h"

class APickup;
class APickupWeapon;
class UWeaponDataAsset;

UCLASS()
class UNREALCPP_API UPickupFactorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void SpawnItem(EItemCode ItemCode, FVector Location, FRotator Rotation, FVector velocity);
	UFUNCTION(BlueprintCallable)
	void SpawnUsedWeaponByItemCode(EItemCode ItemCode, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	APickup* SpawnCurrentWeaponByItemCode(EItemCode ItemCode, FVector Location, FRotator Rotation, FVector Velocity);

private:
	bool IsValidItemCode(EItemCode ItemCode);

protected:
	UPROPERTY(EditAnywhere, Category = "Item")
	TMap<EItemCode, TObjectPtr<UWeaponDataAsset>> ItemCodeToDataAsset;
};
