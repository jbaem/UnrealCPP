#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"

#include "EnhancedWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUseEnded);

UCLASS()
class UNREALCPP_API AEnhancedWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	virtual void OnAttack() override;
	virtual void OnWeaponPickuped(AActionCharacter* Picker) override;
	virtual bool CanAttack() override { return UseCountRemain > 0; }
	virtual int32 GetUsedCountRemain() override { return UseCountRemain; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	int32 UseCountMax = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	int32 UseCountRemain = 10;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Weapon")
	FOnWeaponUseEnded OnWeaponUseEnded;
};
