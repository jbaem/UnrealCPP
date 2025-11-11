// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "ActionPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;


/**
 * 
 */
UCLASS()
class UNREALCPP_API AActionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	//UInputMappingContext a;	// 전방 선언한 것은 헤더가 없으면 인스턴스를 만들 수 없다.
	//UInputMappingContext* b;	// 전방 선언한 것은 주소는 저장할 수 있다.(모든 타입의 주소의 크기는 같으니까)

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;

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

private:
	void OnLookInput(const FInputActionValue& InValue);

	int32 GameInputPriority = 1;
};
