#include "GHAddTagsAnimNotifyState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "../../Log/GHLog.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

void UGHAddTagsAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		IAbilitySystemInterface* asi = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
		if (asi)
		{
			UAbilitySystemComponent* asc = asi->GetAbilitySystemComponent();
			if (asc)
			{
				if (ReplicateTags)
				{
					if (asc->GetOwnerRole() == ROLE_Authority)
					{
						asc->AddReplicatedLooseGameplayTags(AddTagContainer);
					}
				}
				else
				{
					asc->AddLooseGameplayTags(AddTagContainer);
				}

				for (const auto& event_tag : AddTagContainer.GetGameplayTagArray())
				{
					FGameplayEventData payload;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), event_tag, payload);
				}
			}
		}
	}
}

void UGHAddTagsAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		IAbilitySystemInterface* asi = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
		if (asi)
		{
			UAbilitySystemComponent* asc = asi->GetAbilitySystemComponent();
			if (asc)
			{
				if (ReplicateTags)
				{
					if (asc->GetOwnerRole() == ROLE_Authority)
					{
						asc->RemoveReplicatedLooseGameplayTags(AddTagContainer);
					}
				}
				else
				{
					asc->RemoveLooseGameplayTags(AddTagContainer);
				}
				LOG_INFO("ANS移除Tag: %s", *AddTagContainer.ToString());
			}
		}
	}
}

#if WITH_EDITOR
FString UGHAddTagsAnimNotifyState::GetNotifyName_Implementation() const
{
	if (AddTagContainer.IsValidIndex(0))
	{
		return AddTagContainer.GetByIndex(0).ToString();
	}
	return FString("EmptyTag");
}
#endif
