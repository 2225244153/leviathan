// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GHAbilityTaskPlayMontageAndWait.generated.h"

/**
 * 
 */
UCLASS()
class LEVIATHAN_API UGHAbilityTaskPlayMontageAndWait : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable,
		meta = (DisplayName="GHAbilityTaskPlayMontageAndWait", HidePin = "owning_ability", DefaultToSelf = "owning_ability",
			BlueprintInternalUseOnly = "TRUE"))
	static UGHAbilityTaskPlayMontageAndWait* CreateGHPlayMontageAndWaitProxy(
		UGameplayAbility* owning_ability,
		FName task_instance_name,
		UAnimMontage* montage_to_play,
		float play_rate = 1.f,
		FName start_section = NAME_None,
		bool stop_when_ability_ends = true,
		float anim_root_motion_translation_scale = 1.f,
		float start_time_seconds = 0.f);
};
