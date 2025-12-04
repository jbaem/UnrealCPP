#include "Combat/AnimNotify/AnimNotify_AreaAttack.h"
#include "Weapon/Manager/WeaponManagerComponent.h"
#include "Player/Base/ActionCharacter.h"
#include "Player/Base/ActionPlayerController.h"

void UAnimNotify_AreaAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UWeaponManagerComponent* WeaponManagerComponent = OwnerActor->FindComponentByClass<UWeaponManagerComponent>())
		{
			WeaponManagerComponent->OnAreaAttack();
		}
	}
}
