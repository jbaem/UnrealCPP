#include "Combat/AnimNotify/AnimNotifyState_AttackTrace.h"

#include "Player/Base/ActionCharacter.h"

void UAnimNotifyState_AttackTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Owner = Cast<AActionCharacter>(MeshComp->GetOwner());
	if(Owner.IsValid())
	{
		Owner->SetAttackTraceNotify(this);
	}
}

void UAnimNotifyState_AttackTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(Owner.IsValid())
	{
		Owner->SetAttackTraceNotify(nullptr);
		Owner = nullptr;
	}
}
