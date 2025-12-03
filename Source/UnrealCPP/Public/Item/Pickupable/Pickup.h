#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Common/CommonEnums.h"
#include "Core/Interface/Pickable.h"

#include "Pickup.generated.h"

UCLASS()
class UNREALCPP_API APickup : public AActor, public IPickable
{
	GENERATED_BODY()
public:	
	APickup();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void OnPickup_Implementation(AActor* Target) override;
	virtual void OnPickupComplete_Implementation() override;

	void AddImpulse(FVector& Impulse);

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
	float PickupTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	float PickupDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	float PickupHeight = 100.0f;

	UPROPERTY()
	TWeakObjectPtr<AActor> PickupTarget = nullptr;

private:
	FTimerHandle PickupTimerHandle;
	FVector PickupStartLocation = FVector::ZeroVector;
	bool bCanPickedUp = false;
};
