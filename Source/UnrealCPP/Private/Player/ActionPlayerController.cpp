#include "Player/ActionPlayerController.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/MainHUD.h"
#include "UI/MainHudWidget.h"

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

	MainHudWidget = Cast<UMainHudWidget>(GetHUD<AMainHUD>()->GetMainWidget());
}

void AActionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Enhanced Input System 사용
	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(InputComponent);

	if (enhanced)
	{
		enhanced->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AActionPlayerController::OnLookInput);
	}

	if (enhanced)
	{
		enhanced->BindAction(IA_InventoryToggle, ETriggerEvent::Started, this, &AActionPlayerController::OnInventoryToggleInput);
	}
}

void AActionPlayerController::OnLookInput(const FInputActionValue& InValue)
{
	FVector2D lookAxis = InValue.Get<FVector2D>();
	AddYawInput(lookAxis.X);
	AddPitchInput(lookAxis.Y);
}

void AActionPlayerController::OnInventoryToggleInput(const FInputActionValue& InValue)
{
	if(MainHudWidget.IsValid())
	{
		if(MainHudWidget->GetInventoryState() == EInventoryState::EIS_Close)
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Opened"));
			OpenInventoryWidget();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory Closed"));
			CloseInventoryWidget();
		}
	}
}

void AActionPlayerController::SetGameInputMode()
{
	FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
}

void AActionPlayerController::SetInventoryInputMode()
{
	FInputModeGameAndUI inputMode;
	inputMode.SetWidgetToFocus(MainHudWidget->TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputMode.SetHideCursorDuringCapture(false);
	SetInputMode(inputMode);
}

void AActionPlayerController::OpenInventoryWidget()
{
	if(MainHudWidget.IsValid())
	{
		MainHudWidget->OpenInventory();
		
		SetInventoryInputMode();
		bShowMouseCursor = true;
		SetIgnoreMoveInput(true);
		SetIgnoreLookInput(true);
		//SetPause(true);
	}
}

void AActionPlayerController::CloseInventoryWidget()
{
	if(MainHudWidget.IsValid())
	{
		//SetPause(false);
		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(false);
		SetGameInputMode();
		bShowMouseCursor = false;

		MainHudWidget->CloseInventory();
	}
}

void AActionPlayerController::OnAreaAttack()
{
	if (!AreaAttackCameraShakeClass) return;

	PlayerCameraManager->StartCameraShake(
		AreaAttackCameraShakeClass,
		1.0f
	);
}