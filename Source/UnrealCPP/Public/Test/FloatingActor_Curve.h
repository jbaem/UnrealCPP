#pragma once

#include "CoreMinimal.h"
#include "Test/FloatingActor.h"

#include "FloatingActor_Curve.generated.h"

UCLASS()
class UNREALCPP_API AFloatingActor_Curve : public AFloatingActor
{
	GENERATED_BODY()
protected:
	virtual void OnFloatingMeshUpdate(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCurveFloat> HeightCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Interval = 1.0f;
};
