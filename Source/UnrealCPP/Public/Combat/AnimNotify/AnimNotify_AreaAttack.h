#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "AnimNotify_AreaAttack.generated.h"

UCLASS()
class UNREALCPP_API UAnimNotify_AreaAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
private:
	TWeakObjectPtr<class AActionCharacter> OwnerCharacter = nullptr;
};
