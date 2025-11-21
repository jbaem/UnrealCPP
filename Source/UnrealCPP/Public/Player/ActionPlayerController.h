#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"

#include "ActionPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class UNREALCPP_API AActionPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

	void OnAreaAttack();

protected:
	// UPROPERTY는 블루프린트에서 사용할 것 같다 or 가비지 컬렉팅이 필요할 것 같다 => 무조건 붙여준다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Pitch")
	float ViewPitchMax = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Pitch")
	float ViewPitchMin = -40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera|Shake")
	TSubclassOf<UCameraShakeBase> AreaAttackCameraShakeClass;

private:
	void OnLookInput(const FInputActionValue& InValue);

	int32 GameInputPriority = 1;
};
