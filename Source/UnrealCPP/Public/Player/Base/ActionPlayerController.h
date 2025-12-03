#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"

#include "ActionPlayerController.generated.h"

class UEnhancedInputComponent;

UCLASS()
class UNREALCPP_API AActionPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void SetupInputComponent() override;

public:
	void OnAreaAttack();
	void InitializeMainHudWidget(class UMainHudWidget* Widget);

protected:
	void OnLookInput(const FInputActionValue& InValue);
	void OnInventoryToggleInput(const FInputActionValue& InValue);

	// UPROPERTY는 블루프린트에서 사용할 것 같다 or 가비지 컬렉팅이 필요할 것 같다 => 무조건 붙여준다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_InventoryToggle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Pitch")
	float ViewPitchMax = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Pitch")
	float ViewPitchMin = -40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Shake")
	TSubclassOf<UCameraShakeBase> AreaAttackCameraShakeClass;

private:
	void InitMappingContext();
	void SetCameraPitchRange();

	void SetInventoryByPlayer(APawn* InPawn);
	void UnsetInventory();

	void BindActions();
	void BindActionLook(UEnhancedInputComponent* enhanced);
	void BindActionInventoryToggle(UEnhancedInputComponent* enhanced);

	void InitInventoryWidget();
	void BindCloseInventoryToMainHud();
	void OpenInventoryWidget();
	UFUNCTION()
	void CloseInventoryWidget();

	void SetGameInputMode();
	void SetInventoryInputMode();
	void SetIgnoreAll(bool bIsIgnore);

	int32 GameInputPriority = 1;

	TWeakObjectPtr<class UMainHudWidget> MainHudWidget = nullptr;
	TWeakObjectPtr<class UInventoryWidget> InventoryWidget = nullptr;
	TWeakObjectPtr<class UInventoryComponent> InventoryComponent = nullptr;
};
