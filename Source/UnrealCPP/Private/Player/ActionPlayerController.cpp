// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionPlayerController.h"
#include "EnhancedInputSubSystems.h"
#include "InputMappingContext.h"

void AActionPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//LocalPlayer 는 여러 명일 수 있다 (온라인)
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if (Subsystem) // Subsystem이 null이 아니면
	{
		Subsystem->AddMappingContext(DefaultMappingContext, GameInputPriority);
	}
}
