#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Core/Interface/Consumable.h"

#include "EnhancedWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUseEnded);

UCLASS()
class UNREALCPP_API AEnhancedWeapon : public AWeapon
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void OnAttack() override;
	virtual void OnWeaponPickuped() override;
	virtual bool CanAttack() override { return UseCountRemain > 0; }
	virtual int32 GetUsedCountRemain() override { return UseCountRemain; }
	virtual void SetUsedCountRemain(int32 Count) override { UseCountRemain = Count; };

	virtual void OnWeaponActivate() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	int32 UseCountMax = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 UseCountRemain = 10;

	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnConsume OnConsumeDelegate;
};
