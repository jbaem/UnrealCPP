#include "Test/FloatingActor_Curve.h"

void AFloatingActor_Curve::OnFloatingMeshUpdate(float DeltaTime)
{
	if (HeightCurve)
	{
		if (Interval == 0)
		{
			return;
		}
		float curveInput = FMath::Fmod(ElapsedTime / Interval, 1.0f);
		float curveValue = HeightCurve->GetFloatValue(curveInput);
		BodyMesh->SetRelativeLocation(curveValue * MoveHeight * FVector::UpVector);
		BodyMesh->AddRelativeRotation(FRotator(0, SpinSpeed * DeltaTime, 0));
	}
	else
	{
		Super::OnFloatingMeshUpdate(DeltaTime);
	}
}
