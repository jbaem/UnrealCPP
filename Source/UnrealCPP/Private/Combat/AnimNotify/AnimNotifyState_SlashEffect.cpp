#include "Combat/AnimNotify/AnimNotifyState_SlashEffect.h"

#include "Player/Base/ActionCharacter.h"

void UAnimNotifyState_SlashEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	Owner = Cast<AActionCharacter>(MeshComp->GetOwner());
	if (Owner.IsValid())
	{
		Owner->SetSlashEffectNotify(this);
	}
}

void UAnimNotifyState_SlashEffect::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (Owner.IsValid())
	{
		Owner->SetSlashEffectNotify(nullptr);
		Owner = nullptr;
	}
}
