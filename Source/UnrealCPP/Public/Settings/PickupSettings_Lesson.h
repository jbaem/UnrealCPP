#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Common/CommonEnums.h"

#include "PickupSettings_Lesson.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "All Pickup Settings"))
class UNREALCPP_API UPickupSettings_Lesson : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Pickup Data")
	TSoftObjectPtr<UDataTable> PickupDataTable = nullptr;
};
