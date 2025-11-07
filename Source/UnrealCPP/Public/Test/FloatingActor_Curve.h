// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/FloatingActor.h"
#include "FloatingActor_Curve.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API AFloatingActor_Curve : public AFloatingActor
{
	GENERATED_BODY()

protected:
	// if curve is valid then use this, else use parent class function
	virtual void OnFloatingMeshUpdate(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> HeightCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Interval = 1.0f;
};
