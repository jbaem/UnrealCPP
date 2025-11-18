#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickable.h"
#include "Common/CommonEnums.h"

#include "WeaponPickUp.generated.h"

UCLASS()
class UNREALCPP_API AWeaponPickUp : public AActor, public IPickable
{
	GENERATED_BODY()
public:	
	AWeaponPickUp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnPickup_Implementation(AActor* Target) override;

private:
	UFUNCTION()
	void OnTimelineUpdate(float Value);
	
	UFUNCTION()
	void OnTimelineFinished();

protected:
	// for physics simulation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> BaseRoot = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> PickupOverlap = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UNiagaraComponent> Effect = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UTimelineComponent> PickupTimeline = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	EItemCode ItemCode = EItemCode::EIC_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RotateSpeed = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UCurveFloat> ScaleCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UCurveFloat> DistanceCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UCurveFloat> HeightCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	float PickupDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	float PickupHeight = 100.0f;

private:
	TWeakObjectPtr<AActor> PickupTarget = nullptr;

	FVector PickupStartLocation = FVector::ZeroVector;

	bool bIsPickedUp = false;
};
