// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	AWeaponPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnPickup_Implementation(AActor* Target) override;

private:
	UFUNCTION()
	void OnPickupBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void HandleScaleProgress(float Value);
	
	UFUNCTION()
	void HandleDistanceProgress(float Value);

	UFUNCTION()
	void HandleHeightProgress(float Value);

	UFUNCTION()
	void OnScaleFinished();

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
	FVector PickupProgressLocation = FVector::ZeroVector;

	bool bIsPickedUp = false;
};
