#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "DamagePopupSubsystem.generated.h"

class ADamagePopupActor;

/**
 * Damage Popup Subsystem for object pooling
 */
UCLASS()
class UNREALCPP_API UDamagePopupSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// 게임 시작 전(로딩되는 타임)에 서브시스템 초기화
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "Damage Popup Pool")
	void ShowDamagePopup(float Damage, FVector Location);
	UFUNCTION(BlueprintCallable, Category = "Damage Popup Pool")
	void ReturnToPool(ADamagePopupActor* DamagePopup);

protected:
	UPROPERTY()
	TSubclassOf<ADamagePopupActor> PopupClass;

private:
	//UPROPERTY 를 붙이면 직렬화돼서 게임 저장 시 풀 상태가 유지됨
	//Transient 를 붙이면 직렬화에서 제외됨
	UPROPERTY()
	TArray<TObjectPtr<ADamagePopupActor>> Pool;

};
