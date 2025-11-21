#include "AnimNotify/AnimNotify_AreaAttack.h"

#include "Player/ActionCharacter.h"
#include "Player/ActionPlayerController.h"

void UAnimNotify_AreaAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_AreaAttack::Notify called"));

	OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	if(OwnerCharacter.IsValid())
	{
		OwnerCharacter->OnAreaAttack();
		Cast<AActionPlayerController>(OwnerCharacter->GetController())->OnAreaAttack();
	}
}
