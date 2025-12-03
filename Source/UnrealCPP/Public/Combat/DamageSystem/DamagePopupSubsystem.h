#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "DamagePopupSubsystem.generated.h"

class ADamagePopup;

/**
 * Damage Popup Subsystem for object pooling
 */
UCLASS()
class UNREALCPP_API UDamagePopupSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override; 	// 게임 시작 전(로딩되는 타임)에 서브시스템 초기화

public:
	UFUNCTION(BlueprintCallable, Category = "Damage Popup Pool")
	void ShowDamagePopup(FVector InLocation, float InDamage);
	UFUNCTION(BlueprintCallable, Category = "Damage Popup Pool")
	void ReturnToPool(ADamagePopup* InDamagePopup);

private:
	void LoadDamagePopupClass();
	void ActivateDamagePopup(ADamagePopup* InDamagePopup, FVector& InLocation, float InDamage);
	ADamagePopup* GetDamagePopupInPool();
	ADamagePopup* GetNewDamagePopup();
	ADamagePopup* CreateDamagePopupByParams(FActorSpawnParameters& InParams);
	void SetSpawnParamsForPool(FActorSpawnParameters& OutSpawnParams);

protected:
	UPROPERTY()
	TSubclassOf<ADamagePopup> DamagePopupClass;

private:
	//UPROPERTY 를 붙이면 직렬화돼서 게임 저장 시 풀 상태가 유지됨
	//Transient 를 붙이면 직렬화에서 제외됨
	UPROPERTY()
	TArray<TObjectPtr<ADamagePopup>> Pool;
};
