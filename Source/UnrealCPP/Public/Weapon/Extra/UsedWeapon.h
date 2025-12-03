#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "UsedWeapon.generated.h"

UCLASS()
class UNREALCPP_API AUsedWeapon : public AActor
{
	GENERATED_BODY()
public:	
	AUsedWeapon();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> ForceLocation = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Force = 300.0f;
};
