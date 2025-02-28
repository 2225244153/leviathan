// Copyright Epic Games, Inc. All Rights Reserved.

#include "GHAbilityTaskApplyRootMotionConstantForce.h"
#include "GameFramework/RootMotionSource.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemLog.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Leviathan/GHGameFrameWork/GHBaseCharacter.h"

bool FGHRootMotionSource_ConstantForce::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	if (!FRootMotionSource::NetSerialize(Ar, Map, bOutSuccess))
	{
		return false;
	}

	Ar << Force;
	bOutSuccess = true;
	return true;
}

FRootMotionSource* FGHRootMotionSource_ConstantForce::Clone() const
{
	return new FGHRootMotionSource_ConstantForce(*this);
}

UScriptStruct* FGHRootMotionSource_ConstantForce::GetScriptStruct() const
{
	return FGHRootMotionSource_ConstantForce::StaticStruct();
}

void FGHRootMotionSource_ConstantForce::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(StrengthOverTime);
	FRootMotionSource::AddReferencedObjects(Collector);
}

UGHAbilityTaskApplyRootMotionConstantForce* UGHAbilityTaskApplyRootMotionConstantForce::ApplyRootMotionConstantForce
(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	FVector WorldDirection,
	float Strength,
	float Duration,
	bool bIsAdditive,
	FName CurveFloatName,
	FGameplayTag MontageTag,
	ERootMotionFinishVelocityMode VelocityOnFinishMode,
	FVector SetVelocityOnFinish,
	float ClampVelocityOnFinish,
	bool bEnableGravity
)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Duration(Duration);

	UGHAbilityTaskApplyRootMotionConstantForce* apply_root_motion_task = NewAbilityTask<UGHAbilityTaskApplyRootMotionConstantForce>(
		OwningAbility, TaskInstanceName);

	apply_root_motion_task->ForceName = TaskInstanceName;
	apply_root_motion_task->WorldDirection = WorldDirection.GetSafeNormal();
	apply_root_motion_task->Strength = Strength;
	apply_root_motion_task->Duration = Duration;
	apply_root_motion_task->bIsAdditive = bIsAdditive;
	apply_root_motion_task->CurveFloatName = CurveFloatName;
	apply_root_motion_task->MontageTag = MontageTag;
	apply_root_motion_task->FinishVelocityMode = VelocityOnFinishMode;
	apply_root_motion_task->FinishSetVelocity = SetVelocityOnFinish;
	apply_root_motion_task->FinishClampVelocity = ClampVelocityOnFinish;
	apply_root_motion_task->bEnableGravity = bEnableGravity;
	apply_root_motion_task->SharedInitAndApply();

	return apply_root_motion_task;
}


const FFloatCurve* UGHAbilityTaskApplyRootMotionConstantForce::FindHitRecoverCurve(const FName& curve_name)
{
	FGHAnimMontageInfo* montage_info = nullptr;
	if (AGHBaseCharacter* anim_supply_interface = Cast<AGHBaseCharacter>(GetAvatarActor()))
	{
		montage_info = anim_supply_interface->GetAnimMontageInfo(MontageTag);
	}

	if (montage_info)
	{
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


void UGHAbilityTaskApplyRootMotionConstantForce::SharedInitAndApply()
{
	UAbilitySystemComponent* asc = AbilitySystemComponent.Get();
	if (asc && asc->AbilityActorInfo->MovementComponent.IsValid())
	{
		MovementComponent = Cast<UCharacterMovementComponent>(asc->AbilityActorInfo->MovementComponent.Get());
		StartTime = GetWorld()->GetTimeSeconds();
		EndTime = StartTime + Duration;

		const FFloatCurve* fc = FindHitRecoverCurve(CurveFloatName);
		UCurveFloat* ufc = nullptr;
		if (fc)
		{
			if (ufc == nullptr)
			{
				ufc = NewObject<UCurveFloat>(this, TEXT("TaskCurveFloat"), RF_Transactional);
			}

			ufc->FloatCurve = fc->FloatCurve;
		}

		if (MovementComponent)
		{
			ForceName = ForceName.IsNone() ? FName("AbilityTaskApplyRootMotionConstantForce") : ForceName;
			TSharedPtr<FGHRootMotionSource_ConstantForce> constant_force = MakeShared<
				FGHRootMotionSource_ConstantForce>();
			constant_force->InstanceName = ForceName;
			constant_force->AccumulateMode = bIsAdditive
				                                 ? ERootMotionAccumulateMode::Additive
				                                 : ERootMotionAccumulateMode::Override;
			constant_force->Priority = 5;
			constant_force->Force = WorldDirection * Strength;
			constant_force->Duration = Duration;
			constant_force->StrengthOverTime = ufc;
			constant_force->FinishVelocityParams.Mode = FinishVelocityMode;
			constant_force->FinishVelocityParams.SetVelocity = FinishSetVelocity;
			constant_force->FinishVelocityParams.ClampVelocity = FinishClampVelocity;
			if (bEnableGravity)
			{
				constant_force->Settings.SetFlag(ERootMotionSourceSettingsFlags::IgnoreZAccumulate);
			}
			RootMotionSourceID = MovementComponent->ApplyRootMotionSource(constant_force);
		}
	}
}

void UGHAbilityTaskApplyRootMotionConstantForce::TickTask(float DeltaTime)
{
	if (bIsFinished)
	{
		return;
	}

	Super::TickTask(DeltaTime);

	if (AActor* my_actor = GetAvatarActor())
	{
		const bool b_timed_out = HasTimedOut();
		const bool b_is_infinite_duration = Duration < 0.f;

		if (!b_is_infinite_duration && b_timed_out)
		{
			// Task has finished
			bIsFinished = true;
			if (!bIsSimulating)
			{
				my_actor->ForceNetUpdate();
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnFinish.Broadcast();
				}
				EndTask();
			}
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}

void UGHAbilityTaskApplyRootMotionConstantForce::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGHAbilityTaskApplyRootMotionConstantForce, WorldDirection);
	DOREPLIFETIME(UGHAbilityTaskApplyRootMotionConstantForce, Strength);
	DOREPLIFETIME(UGHAbilityTaskApplyRootMotionConstantForce, Duration);
	DOREPLIFETIME(UGHAbilityTaskApplyRootMotionConstantForce, bIsAdditive);
	DOREPLIFETIME(UGHAbilityTaskApplyRootMotionConstantForce, bEnableGravity);
	DOREPLIFETIME(UGHAbilityTaskApplyRootMotionConstantForce, MontageTag);
	DOREPLIFETIME(UGHAbilityTaskApplyRootMotionConstantForce, CurveFloatName);
}

void UGHAbilityTaskApplyRootMotionConstantForce::PreDestroyFromReplication()
{
	bIsFinished = true;
	EndTask();
}

void UGHAbilityTaskApplyRootMotionConstantForce::OnDestroy(bool AbilityIsEnding)
{
	if (MovementComponent)
	{
		MovementComponent->RemoveRootMotionSourceByID(RootMotionSourceID);
	}

	Super::OnDestroy(AbilityIsEnding);
}
