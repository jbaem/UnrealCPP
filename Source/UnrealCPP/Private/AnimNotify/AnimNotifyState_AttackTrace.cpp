// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_AttackTrace.h"
#include "Player/ActionCharacter.h"

void UAnimNotifyState_AttackTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	if(OwnerCharacter.IsValid())
	{
		OwnerCharacter->SetAttackTraceNotify(this);
	}
}

void UAnimNotifyState_AttackTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if(OwnerCharacter.IsValid())
	{
		// OwnerCharacter 설정 해제
		OwnerCharacter->SetAttackTraceNotify(nullptr);
		OwnerCharacter = nullptr;
	}


}
