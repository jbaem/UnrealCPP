#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonEnums.h"
#include "Data/WeaponDataAsset.h"

#include "WeaponManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UWeaponManagerComponent();

protected:
	virtual void BeginPlay() override;

public:	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Database")
	TMap<EItemCode, TObjectPtr<UWeaponDataAsset>> WeaponDatabase;
};
