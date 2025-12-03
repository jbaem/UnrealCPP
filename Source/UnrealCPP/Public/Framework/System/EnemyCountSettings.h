#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Enemy/Base/EnemyPawn.h"

#include "EnemyCountSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Enemy Count Settings"))
class UNREALCPP_API UEnemyCountSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<AEnemyPawn> EnemyClass;
};
