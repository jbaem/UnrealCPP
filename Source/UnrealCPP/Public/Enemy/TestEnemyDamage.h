#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "TestEnemyDamage.generated.h"

UCLASS()
class UNREALCPP_API ATestEnemyDamage : public APawn
{
	GENERATED_BODY()

public:
	ATestEnemyDamage();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> PopupLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class ADamagePopupActor> DamagePopupClass;
};
