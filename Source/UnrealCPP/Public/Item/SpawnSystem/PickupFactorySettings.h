#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Core/Common/CommonEnums.h"
#include "Weapon/Data/WeaponDataAsset.h"

#include "PickupFactorySettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Pickup Factory Settings"))
class UNREALCPP_API UPickupFactorySettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere)
	TMap<EItemCode, TSoftObjectPtr<UWeaponDataAsset>> ItemCodeToDataAsset;
};
