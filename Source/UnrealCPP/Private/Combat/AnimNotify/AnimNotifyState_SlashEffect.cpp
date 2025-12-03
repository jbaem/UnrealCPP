#include "Combat/AnimNotify/AnimNotifyState_SlashEffect.h"

#include "Player/Base/ActionCharacter.h"

void UAnimNotifyState_SlashEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->SetSlashEffectNotify(this);
	}
}

void UAnimNotifyState_SlashEffect::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->SetSlashEffectNotify(nullptr);
		OwnerCharacter = nullptr;
	}
}
