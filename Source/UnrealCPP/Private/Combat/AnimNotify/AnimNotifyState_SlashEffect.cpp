#include "Combat/AnimNotify/AnimNotifyState_SlashEffect.h"
#include "Weapon/Manager/WeaponManagerComponent.h"
#include "Player/Base/ActionCharacter.h"

void UAnimNotifyState_SlashEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UWeaponManagerComponent* WeaponManagerComponent = OwnerActor->FindComponentByClass<UWeaponManagerComponent>())
		{
			WeaponManagerComponent->ActivateSlashEffect(true);
		}
	}
}

void UAnimNotifyState_SlashEffect::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UWeaponManagerComponent* WeaponManagerComponent = OwnerActor->FindComponentByClass<UWeaponManagerComponent>())
		{
			WeaponManagerComponent->ActivateSlashEffect(false);
		}
	}
}
