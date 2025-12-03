#include "Combat/AnimNotify/AnimNotify_AreaAttack.h"

#include "Player/Base/ActionCharacter.h"
#include "Player/Base/ActionPlayerController.h"

void UAnimNotify_AreaAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	Owner = Cast<AActionCharacter>(MeshComp->GetOwner());
	if(Owner.IsValid())
	{
		Owner->OnAreaAttack();
		Cast<AActionPlayerController>(Owner->GetController())->OnAreaAttack();
	}
}
