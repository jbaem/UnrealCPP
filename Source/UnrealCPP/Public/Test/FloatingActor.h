#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FloatingActor.generated.h"

UCLASS()
class UNREALCPP_API AFloatingActor : public AActor
{
	GENERATED_BODY()
public:	
	AFloatingActor();

protected:
	virtual void BeginPlay() override;

	virtual void OnFloatingMeshUpdate(float DeltaTime);

	void ChangeMoveDirection();

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeedRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpinSpeed = 0.0f;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BodyMesh = nullptr;

	bool bIsMoveUp = true;
	float ElapsedTime = 0.0f;
};
