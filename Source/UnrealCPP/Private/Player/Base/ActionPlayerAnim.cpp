#include "Player/Base/ActionPlayerAnim.h"

#include "GameFramework/PawnMovementComponent.h"

void UActionPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	InitOwner();
}

void UActionPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateByOwnerProperty();
}

void UActionPlayerAnim::UpdateByOwnerProperty()
{
	if (!::IsValid(OwnerPawn)) return;
	UpdateSpeed();
	UpdateDirection();
}

void UActionPlayerAnim::InitOwner()
{
	OwnerPawn = TryGetPawnOwner();
	if (OwnerPawn)
	{
		MovementComponent = OwnerPawn->GetMovementComponent();
	}
}

void UActionPlayerAnim::UpdateSpeed()
{
	//Speed = OwnerPawn->GetVelocity().Size();
	Speed = MovementComponent ? MovementComponent->Velocity.Size() : 0.0f;
}

void UActionPlayerAnim::UpdateDirection()
{
	Direction = CalculateDirection(OwnerPawn->GetVelocity(), OwnerPawn->GetActorRotation());
}
