// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionPlayerAnim.h"

void UActionPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerPawn = TryGetPawnOwner();
}

void UActionPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!::IsValid(OwnerPawn)) return;

	Speed = OwnerPawn->GetVelocity().Size();
	Direction = CalculateDirection(OwnerPawn->GetVelocity(), OwnerPawn->GetActorRotation());
}
