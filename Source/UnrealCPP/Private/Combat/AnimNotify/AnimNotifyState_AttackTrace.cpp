#include "Combat/AnimNotify/AnimNotifyState_AttackTrace.h"
#include "Weapon/Manager/WeaponManagerComponent.h"

#include "Player/Base/ActionCharacter.h"

void UAnimNotifyState_AttackTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UWeaponManagerComponent* WeaponManagerComponent = OwnerActor->FindComponentByClass<UWeaponManagerComponent>())
		{
			WeaponManagerComponent->AttackEnable(true);
		}
	}
}

void UAnimNotifyState_AttackTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UWeaponManagerComponent* WeaponManagerComponent = OwnerActor->FindComponentByClass<UWeaponManagerComponent>())
		{
			WeaponManagerComponent->AttackEnable(false);
		}
	}
}
