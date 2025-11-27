#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Item/Pickup.h"
#include "Common/CommonEnums.h"

#include "PickupFactorySubsystem_Lesson.generated.h"

UCLASS()
class UNREALCPP_API UPickupFactorySubsystem_Lesson : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	APickup* SpawnPickup(EItemCode ItemCode, FVector Location, FRotator Rotator);

protected:
	void LoadPickupClassesMap();

protected:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	TMap<EItemCode, TSubclassOf<APickup>> PickupClasses;
};
