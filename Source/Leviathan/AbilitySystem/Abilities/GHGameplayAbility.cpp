#include "GHGameplayAbility.h"
#include "Leviathan/AbilitySystem/Component/GHAbilitySystemComponent.h"
#include "Leviathan/GHGameFrameWork/GHBaseCharacter.h"
#include "Leviathan/GHManagers/GHCoreDelegatesMgr.h"

DEFINE_LOG_CATEGORY(LogGHGameplayAbility)

UGHGameplayAbility::UGHGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}

UGHAbilitySystemComponent* UGHGameplayAbility::GetAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo
		        ? Cast<UGHAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get())
		        : nullptr);
}

void UGHGameplayAbility::OnAbilityFailedToActivate(const FGameplayTagContainer& FailureReason) const
{
	ScriptOnAbilityFailedToActivate(FailureReason);
}

void UGHGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	UE_LOG(LogGHGameplayAbility, Log, TEXT("技能激活: 技能:%s"), *GetName());

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGHGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                    const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo,
                                    bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogGHGameplayAbility, Log, TEXT("技能结束: 技能:%s 是否被强制打断:%d"), *GetName(), bWasCancelled);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AGHBaseCharacter* owner = Cast<AGHBaseCharacter>(ActorInfo->OwnerActor);
	if (owner != nullptr)
	{
		UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
		coreDelegatesMgr->OnCharacterEndAbility.Broadcast(owner->GetID(), bWasCancelled);
	}
}

void UGHGameplayAbility::EndCurrentAbility(bool is_replicate_end_ability, bool is_was_cancelled)
{
	if (IsActive())
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(),
		           is_replicate_end_ability, is_was_cancelled);
	}
}

bool UGHGameplayAbility::IsServer()
{
	AActor* Actor = GetAvatarActorFromActorInfo();
	if (Actor)
	{
		return Actor->HasAuthority();
	}
	return false;
}
