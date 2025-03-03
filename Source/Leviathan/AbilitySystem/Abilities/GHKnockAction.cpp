#include "GHKnockAction.h"
#include "Leviathan/AbilitySystem/AbilityTask/GHAbilityTaskApplyRootMotionConstantForce.h"
#include "Leviathan/Animation/GHAnimationLib.h"
#include "../../GHComponents/SkillKnockComponent.h"

void UGHKnockAction::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	const FKnockInfo& knock_info = GetKnockComponent()->CurrentKnockInfo;
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	if (knock_info.bForceMove)
	{
		//强制位移
		if (FGHAnimMontageInfo* montage_info = GetAnimMontageInfo(AnimBeforeMontagePlayer.ActionMontageTag))
		{
			UAnimMontage* Montage = Cast<UAnimMontage>(montage_info->AnimMontage.LoadSynchronous());
			if (Montage)
			{
				ApplyRootMotionConstantForceTask =
					UGHAbilityTaskApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
						this, "Knock", knock_info.KnockForceMoveInfo.Direction,
						knock_info.KnockForceMoveInfo.Strength,
						Montage->GetPlayLength() / montage_info->PlayRate,
						knock_info.KnockForceMoveInfo.IsAdditive,
						knock_info.KnockForceMoveInfo.AnimCurveForStrengthName,
						AnimBeforeMontagePlayer.ActionMontageTag, knock_info.KnockForceMoveInfo.VelocityOnFinishMode,
						knock_info.KnockForceMoveInfo.SetVelocityOnFinish,
						knock_info.KnockForceMoveInfo.ClampVelocityOnFinish,
						knock_info.KnockForceMoveInfo.EnableGravity);

				ApplyRootMotionConstantForceTask->ReadyForActivation();
			}
		}
	}
}

void UGHKnockAction::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ApplyRootMotionConstantForceTask)
	{
		ApplyRootMotionConstantForceTask->EndTask();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

USkillKnockComponent* UGHKnockAction::GetKnockComponent()
{
	if (KnockComponent == nullptr)
	{
		AActor* owner_actor = GetAvatarActorFromActorInfo();
		check(owner_actor);

		KnockComponent = owner_actor->FindComponentByClass<USkillKnockComponent>();
	}

	return KnockComponent;
}
