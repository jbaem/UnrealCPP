#include "Player/Base/ActionPlayerController.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/Base/ActionCharacter.h"
#include "Framework/UI/MainHUD.h"
#include "Framework/UI/MainHudWidget.h"
#include "Inventory/UI/InventoryWidget.h"
#include "Inventory/Base/InventoryComponent.h"

void AActionPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	InitMappingContext();
	SetCameraPitchRange();
}

void AActionPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetInventoryByPlayer(InPawn);
}

void AActionPlayerController::OnUnPossess()
{
	UnsetInventory();

	Super::OnUnPossess();
}

void AActionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	BindActions();
}

void AActionPlayerController::OnAreaAttack()
{
	if (!AreaAttackCameraShakeClass) return;

	PlayerCameraManager->StartCameraShake(
		AreaAttackCameraShakeClass,
		1.0f
	);
}

void AActionPlayerController::InitializeMainHudWidget(UMainHudWidget* Widget)
{
	if (!Widget) return;

	MainHudWidget = Widget;
	BindCloseInventoryToMainHud();
	InitInventoryWidget();
}

void AActionPlayerController::OnLookInput(const FInputActionValue& InValue)
{
	FVector2D lookAxis = InValue.Get<FVector2D>();
	AddYawInput(lookAxis.X);
	AddPitchInput(lookAxis.Y);
}

void AActionPlayerController::OnInventoryToggleInput(const FInputActionValue& InValue)
{
	if (!MainHudWidget.IsValid()) return;

	if (MainHudWidget->GetInventoryState() == EInventoryState::EIS_Close)
	{
		OpenInventoryWidget();
	}
	else
	{
		CloseInventoryWidget();
	}
}

void AActionPlayerController::InitMappingContext()
{
	//LocalPlayer 는 여러 명일 수 있다 (온라인)
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) // Subsystem이 null이 아니면
	{
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, GameInputPriority);
	}
}

void AActionPlayerController::SetCameraPitchRange()
{
	PlayerCameraManager->ViewPitchMax = ViewPitchMax;
	PlayerCameraManager->ViewPitchMin = ViewPitchMin;
}

void AActionPlayerController::SetInventoryByPlayer(APawn* InPawn)
{
	if (AActionCharacter* player = Cast<AActionCharacter>(InPawn))
	{
		InventoryComponent = player->GetInventoryComponent();
	}
}

void AActionPlayerController::UnsetInventory()
{
	InventoryComponent = nullptr;
}

void AActionPlayerController::BindActions()
{
	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(InputComponent);
	if (!enhanced) return;

	BindActionLook(enhanced);
	BindActionInventoryToggle(enhanced);
}

void AActionPlayerController::BindActionLook(UEnhancedInputComponent* enhanced)
{
	if (!IA_Look) return;
	enhanced->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AActionPlayerController::OnLookInput);
}

void AActionPlayerController::BindActionInventoryToggle(UEnhancedInputComponent* enhanced)
{
	if (!IA_InventoryToggle) return;
	enhanced->BindAction(IA_InventoryToggle, ETriggerEvent::Started, this, &AActionPlayerController::OnInventoryToggleInput);
}

void AActionPlayerController::InitInventoryWidget()
{
	InventoryWidget = MainHudWidget->GetInventoryWidget();
	if (InventoryWidget.IsValid())
	{
		InventoryWidget->InitializeInventoryWidget(InventoryComponent.Get());
	}
}

void AActionPlayerController::BindCloseInventoryToMainHud()
{
	FScriptDelegate delegate;
	delegate.BindUFunction(this, "CloseInventoryWidget");
	MainHudWidget->AddToInventoryCloseDelegate(delegate);
}


void AActionPlayerController::OpenInventoryWidget()
{
	MainHudWidget->OpenInventory();

	SetInventoryInputMode();
	bShowMouseCursor = true;
	SetIgnoreAll(true);
}

void AActionPlayerController::CloseInventoryWidget()
{
	SetIgnoreAll(false);
	SetGameInputMode();
	bShowMouseCursor = false;

	MainHudWidget->CloseInventory();
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

void AActionPlayerController::SetIgnoreAll(bool bIsIgnore)
{
	SetIgnoreMoveInput(bIsIgnore);
	SetIgnoreLookInput(bIsIgnore);
	//SetPause(bIsIgnore);
}
