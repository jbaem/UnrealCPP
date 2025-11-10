// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionPlayerController.h"
#include "InputMappingContext.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

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

	PlayerCameraManager->ViewPitchMax = ViewPitchMax;
	PlayerCameraManager->ViewPitchMin = ViewPitchMin;
}

void AActionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Enhanced Input System 사용
	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(InputComponent);

	if (enhanced)
	{
		enhanced->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AActionPlayerController::OnLookInput);
		//여기에 Input Action Bindings 추가
	}
}

void AActionPlayerController::OnLookInput(const FInputActionValue& InValue)
{
	FVector2D lookAxis = InValue.Get<FVector2D>();
	AddYawInput(lookAxis.X);
	AddPitchInput(lookAxis.Y);
	//UE_LOG(LogTemp, Log, TEXT("Dir : (%.1f, %.1f)"), lookAxis.X, lookAxis.Y);
	//UE_LOG(LogTemp, Log, TEXT("Dir : (%s)"), *lookAxis.ToString());
}
