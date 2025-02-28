#include "GHKnockAction.h"
#include "Leviathan/AbilitySystem/AbilityTask/GHAbilityTaskApplyRootMotionConstantForce.h"
#include "Leviathan/Animation/GHAnimationLib.h"
#include "../../GHComponents/SkillKnockComponent.h"
#include "Leviathan/Log/GHLog.h"

const FFloatCurve* UGHKnockAction::FindHitRecoverCurve(const FGameplayTag& montage_tag, const FName& curve_name)
{
	if (FGHAnimMontageInfo* montage_info = GetAnimMontageInfo(montage_tag))
	{
		check(montage_info->AnimMontage);
		for (const FFloatCurve& float_curve : montage_info->AnimMontage->GetCurveData().FloatCurves)
		{
			if (float_curve.GetName() == curve_name)
			{
				return &float_curve;
			}
		}
	}

	return nullptr;
}

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
			ApplyRootMotionConstantForceTask = UGHAbilityTaskApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
				this, "Knock", knock_info.KnockForceMoveInfo.Direction,
				knock_info.KnockForceMoveInfo.Strength,
				montage_info->AnimMontage->GetPlayLength() / montage_info->PlayRate,
				knock_info.KnockForceMoveInfo.IsAdditive, knock_info.KnockForceMoveInfo.AnimCurveForStrengthName,
				AnimBeforeMontagePlayer.ActionMontageTag, knock_info.KnockForceMoveInfo.VelocityOnFinishMode,
				knock_info.KnockForceMoveInfo.SetVelocityOnFinish, knock_info.KnockForceMoveInfo.ClampVelocityOnFinish,
				knock_info.KnockForceMoveInfo.EnableGravity);

			ApplyRootMotionConstantForceTask->ReadyForActivation();
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
