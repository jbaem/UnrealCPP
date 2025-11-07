// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class UNREALCPP_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void ChangeMoveDirection();

public:	
	// Called every frame
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

private:
	bool bIsMoveUp = true;
	float ElapsedTime = 0.0f;
};
