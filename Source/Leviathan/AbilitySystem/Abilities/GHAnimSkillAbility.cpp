#include "GHAnimSkillAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Leviathan/AbilitySystem/AbilityTask/GHAbilityTaskPlayMontageAndWait.h"
#include "Leviathan/AbilitySystem/TargetActor/GHTargetActor.h"
#include "Leviathan/Animation/GHAnimationLib.h"
#include "Leviathan/GHComponents/SkillKnockComponent.h"
#include "Leviathan/GHGameFrameWork/GHBaseCharacter.h"
#include "Leviathan/Log/GHLog.h"

void UGHAnimSkillAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	LOG_INFO("Actor:%s  激活技能：%s", *GetAvatarActorFromActorInfo()->GetName(), *GetName());
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	WaitHandleDamage();

	// 持续性技能才考虑动画
	if (TargetingConfirmation != EGameplayTargetingConfirmation::UserConfirmed || AnimBeforeMontagePlayer.
		ActionMontageTag.IsValid())
	{
		if (AnimBeforeMontagePlayer.ActionMontageTag.IsValid())
		{
			InitAnimMontagePlayer(AnimBeforeMontagePlayer);
		}
	}
}

void UGHAnimSkillAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                     bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	EndAnimMontagePlayer(AnimBeforeMontagePlayer);
}

FGHAnimMontageInfo* UGHAnimSkillAbility::GetAnimMontageInfo(const FGameplayTag& montage_tag)
{
	if (AGHBaseCharacter* character = Cast<AGHBaseCharacter>(
		GetAvatarActorFromActorInfo()))
	{
		return character->GetAnimMontageInfo(montage_tag);
	}
	return nullptr;
}

void UGHAnimSkillAbility::ActiveTargetActor(TSubclassOf<AGHTargetActor> target_actor_class)
{
	if (target_actor_class)
	{
		SpawnAndWaitTargetData(TargetingConfirmation, target_actor_class);
	}
}

void UGHAnimSkillAbility::SpawnAndWaitTargetData(TEnumAsByte<EGameplayTargetingConfirmation::Type> confirmation_type,
                                                 TSubclassOf<AGHTargetActor> target_actor_class)
{
	CurrentTargetDataTask = UAbilityTask_WaitTargetData::WaitTargetData(this, "Targeting", confirmation_type,
	                                                                    target_actor_class);

	CurrentTargetDataTask->ValidData.AddDynamic(this, &UGHAnimSkillAbility::OnReceiveTargetData);
	CurrentTargetDataTask->Cancelled.AddDynamic(this, &UGHAnimSkillAbility::OnCancelTargetData);

	AGameplayAbilityTargetActor* ability_target_actor;

	if (CurrentTargetDataTask->BeginSpawningActor(this, target_actor_class, ability_target_actor))
	{
		CurrentTargetDataTask->FinishSpawningActor(this, ability_target_actor);
		CurrentTargetActor = Cast<AGHTargetActor>(ability_target_actor);
		CurrentTargetActor->SetActorTransform(GetAvatarActorFromActorInfo()->GetTransform());
	}

	CurrentTargetDataTask->ReadyForActivation();

	if (CurrentTargetActor)
	{
		CurrentTargetActor->ConfirmTargeting();
	}
}

void UGHAnimSkillAbility::OnReceiveTargetData(const FGameplayAbilityTargetDataHandle& data)
{
	TArray<AActor*> target_actors;
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	FGHGameplayEffectContainer* container = EffectContainerMap.Find(PayLoad.EventTag);
	AActor* source_actor = GetAvatarActorFromActorInfo();
	for (const auto& TempData : data.Data)
	{
		FGameplayAbilityTargetData* TargetData = TempData.Get();

		if (IsActive())
		{
			for (const auto& target_actor : TargetData->GetActors())
			{
				if (target_actor == GetAvatarActorFromActorInfo() || !target_actor.IsValid())
				{
					continue;
				}

				target_actors.Emplace(target_actor.Get());

				for (const auto& target_effect_class : container->TargetGameplayEffects)
				{
					FGameplayEffectSpecHandle gameplay_effect_spec_handle = MakeOutgoingGameplayEffectSpec(
						target_effect_class, 1);
					IAbilitySystemInterface* ability_system_interface = Cast<IAbilitySystemInterface>(
						target_actor);
					ability_system_interface->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(
						*gameplay_effect_spec_handle.Data.Get());
				}

				USkillKnockComponent* knock_comp = Cast<USkillKnockComponent>(
					target_actor->GetComponentByClass(USkillKnockComponent::StaticClass()));
				FKnockInfo knock_info;

				knock_info.Direction = target_actor->GetActorLocation() - source_actor->GetActorLocation();
				knock_info.bForceMove = true;
				knock_comp->ActivateKnock(
					knock_info, knock_info.Direction, 100);
			}
		}
	}

	for (const auto& self_effect_class : container->SelfGameplayEffects)
	{
		FGameplayEffectSpecHandle gameplay_effect_spec_handle = MakeOutgoingGameplayEffectSpec(
			self_effect_class, 1);
		IAbilitySystemInterface* ability_system_interface = Cast<IAbilitySystemInterface>(
			GetAvatarActorFromActorInfo());
		ability_system_interface->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(
			*gameplay_effect_spec_handle.Data.Get());
	}
}

void UGHAnimSkillAbility::OnCancelTargetData(const FGameplayAbilityTargetDataHandle& data)
{
	EndCurrentAbility(true, true);
}

void UGHAnimSkillAbility::OnEventReceived(FGameplayEventData payload)
{
	FGHGameplayEffectContainer* effect_container = EffectContainerMap.Find(payload.EventTag);
	if (effect_container)
	{
		PayLoad = payload;

		//激活指示器
		ActiveTargetActor(effect_container->TargetActor);
	}
}

void UGHAnimSkillAbility::EndAnimMontagePlayer(FGHAnimSkillMontagePlayer& anim_skill_montage_player)
{
	if (anim_skill_montage_player.ActionMontagePlayer)
	{
		anim_skill_montage_player.ActionMontagePlayer->OnInterrupted.RemoveAll(this);
		anim_skill_montage_player.ActionMontagePlayer->OnCancelled.RemoveAll(this);
		anim_skill_montage_player.ActionMontagePlayer->OnCompleted.RemoveAll(this);
		anim_skill_montage_player.ActionMontagePlayer->OnBlendOut.RemoveAll(this);

		anim_skill_montage_player.ActionMontagePlayer->EndTask();
		anim_skill_montage_player.ActionMontagePlayer = nullptr;
	}
}

void UGHAnimSkillAbility::WaitHandleDamage()
{
	TArray<FGameplayTag> event_tags;
	EffectContainerMap.GetKeys(event_tags);
	for (const auto& event_tag : event_tags)
	{
		UAbilityTask_WaitGameplayEvent* wait_gameplay_event = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this, event_tag, nullptr, false, true);
		wait_gameplay_event->EventReceived.AddDynamic(this, &UGHAnimSkillAbility::OnEventReceived);
		wait_gameplay_event->ReadyForActivation();
	}
}

bool UGHAnimSkillAbility::InitAnimMontagePlayer(FGHAnimSkillMontagePlayer& anim_skill_montage_player)
{
	FGHAnimMontageInfo* montage_info = GetAnimMontageInfo(anim_skill_montage_player.ActionMontageTag);
	if (montage_info == nullptr)
	{
		LOG_ERROR("无效的动作蒙太奇:%s", *anim_skill_montage_player.ActionMontageTag.ToString());
		EndCurrentAbility(false, false);
		return false;
	}

	if (nullptr != CurrentMontagePlayer)
	{
		EndAnimMontagePlayer(*CurrentMontagePlayer);
	}

	CurrentMontagePlayer = &anim_skill_montage_player;

	EndAnimMontagePlayer(anim_skill_montage_player);

	// 设置当前的动作蒙太奇tag
	CurrentActionMontageTag = anim_skill_montage_player.ActionMontageTag;
	UAnimMontage* Montage = Cast<UAnimMontage>(montage_info->AnimMontage.LoadSynchronous());

	// 创建蒙太奇播放器	
	anim_skill_montage_player.ActionMontagePlayer =
		UGHAbilityTaskPlayMontageAndWait::CreateGHPlayMontageAndWaitProxy(
			this, anim_skill_montage_player.ActionMontageTag.GetTagName(),
			Montage,
			montage_info->PlayRate * anim_skill_montage_player.PlayMontageRate,
			NAME_None,
			anim_skill_montage_player.StopWhenAbilityEnd,
			GetRootMotionScale(),
			anim_skill_montage_player.PlayMontageStartTimeSeconds);

	// 设置回调通知和任务播放
	anim_skill_montage_player.ActionMontagePlayer->OnInterrupted.AddDynamic(
		this, &UGHAnimSkillAbility::OnActionInterrupted);
	anim_skill_montage_player.ActionMontagePlayer->OnCancelled.
	                          AddDynamic(this, &UGHAnimSkillAbility::OnActionCancelled);
	anim_skill_montage_player.ActionMontagePlayer->OnCompleted.
	                          AddDynamic(this, &UGHAnimSkillAbility::OnActionCompleted);
	anim_skill_montage_player.ActionMontagePlayer->OnBlendOut.AddDynamic(this, &UGHAnimSkillAbility::OnActionBlendOut);
	anim_skill_montage_player.ActionMontagePlayer->ReadyForActivation();
	return true;
}

void UGHAnimSkillAbility::OnActionBlendOut_Implementation()
{
	EndCurrentAbility(false, false);
}

void UGHAnimSkillAbility::OnActionCompleted_Implementation()
{
	if (AnimBeforeMontagePlayer.ActionMontagePlayer)
	{
		if (CurrentActionMontageTag == AnimBeforeMontagePlayer.ActionMontageTag)
		{
			EndAnimMontagePlayer(AnimBeforeMontagePlayer);
			EndCurrentAbility(false, false);
		}
		else
		{
			EndCurrentAbility(false, false);
		}
	}
	else
	{
		// 结束了
		EndCurrentAbility(false, false);
	}
}

void UGHAnimSkillAbility::OnActionInterrupted_Implementation()
{
	EndCurrentAbility(false, true);
}

void UGHAnimSkillAbility::OnActionCancelled_Implementation()
{
	EndCurrentAbility(false, true);
}

float UGHAnimSkillAbility::GetRootMotionScale_Implementation()
{
	return 1.f;
}
