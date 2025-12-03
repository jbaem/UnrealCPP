#include "Combat/AnimNotify/AnimNotifyState_SectionJump.h"

#include "Player/Base/ActionCharacter.h"

void UAnimNotifyState_SectionJump::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	Owner = Cast<AActionCharacter>(MeshComp->GetOwner());
	if(Owner.IsValid())
	{
		Owner->SetSectionJumpNotify(this);
	}
}

void UAnimNotifyState_SectionJump::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Owner.IsValid())
	{
		Owner->SetSectionJumpNotify(nullptr);
		Owner = nullptr;
	}
}
