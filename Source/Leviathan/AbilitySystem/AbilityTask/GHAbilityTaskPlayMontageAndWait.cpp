// Fill out your copyright notice in the Description page of Project Settings.


#include "GHAbilityTaskPlayMontageAndWait.h"
#include "AbilitySystemGlobals.h"

UGHAbilityTaskPlayMontageAndWait* UGHAbilityTaskPlayMontageAndWait::CreateGHPlayMontageAndWaitProxy(
	UGameplayAbility* owning_ability,
	FName task_instance_name,
	UAnimMontage* montage_to_play,
	float rate,
	FName start_section,
	bool stop_when_ability_ends,
	float anim_root_motion_translation_scale,
	float start_time_seconds)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(rate);
	UGHAbilityTaskPlayMontageAndWait* task = NewAbilityTask<UGHAbilityTaskPlayMontageAndWait>(
		owning_ability, task_instance_name);
	task->MontageToPlay = montage_to_play;
	task->Rate = rate;
	task->StartSection = start_section;
	task->AnimRootMotionTranslationScale = anim_root_motion_translation_scale;
	task->bStopWhenAbilityEnds = stop_when_ability_ends;
	task->StartTimeSeconds = start_time_seconds;
	return task;
}
