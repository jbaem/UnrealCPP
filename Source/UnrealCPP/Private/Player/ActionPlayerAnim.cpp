#include "Player/ActionPlayerAnim.h"

#include "GameFramework/PawnMovementComponent.h"

void UActionPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerPawn = TryGetPawnOwner();
	if (OwnerPawn)
	{
		MovementComponent = OwnerPawn->GetMovementComponent();
	}
}

void UActionPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!::IsValid(OwnerPawn)) return;

	//Speed = OwnerPawn->GetVelocity().Size();
	Speed = MovementComponent ? MovementComponent->Velocity.Size() : 0.0f;

	Direction = CalculateDirection(OwnerPawn->GetVelocity(), OwnerPawn->GetActorRotation());
}
