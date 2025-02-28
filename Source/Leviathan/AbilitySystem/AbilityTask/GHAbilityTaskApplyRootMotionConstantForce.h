// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotion_Base.h"
#include "GameFramework/RootMotionSource.h"
#include "GHAbilityTaskApplyRootMotionConstantForce.generated.h"

class UCharacterMovementComponent;
class UCurveFloat;
class UGameplayTasksComponent;
class AActor;

USTRUCT()
struct FGHRootMotionSource_ConstantForce : public FRootMotionSource_ConstantForce
{
	GENERATED_USTRUCT_BODY()
	
	FGHRootMotionSource_ConstantForce(){}
	virtual ~FGHRootMotionSource_ConstantForce() {}
		
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	virtual FRootMotionSource* Clone() const override;

	virtual UScriptStruct* GetScriptStruct() const override;
	virtual void AddReferencedObjects(class FReferenceCollector& Collector) override;
};

template<>
struct TStructOpsTypeTraits< FGHRootMotionSource_ConstantForce > : public TStructOpsTypeTraitsBase2< FGHRootMotionSource_ConstantForce >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

/**
 *	Applies force to character's movement
 */
UCLASS()
class LEVIATHAN_API UGHAbilityTaskApplyRootMotionConstantForce : public UAbilityTask_ApplyRootMotion_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FApplyRootMotionConstantForceDelegate OnFinish;

	/** Apply force to character's movement */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGHAbilityTaskApplyRootMotionConstantForce* ApplyRootMotionConstantForce
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
	);
	const FFloatCurve* FindHitRecoverCurve(const FName& curve_name);

	/** Tick function for this task, if bTickingTask == true */
	virtual void TickTask(float DeltaTime) override;
	virtual void PreDestroyFromReplication() override;
	virtual void OnDestroy(bool AbilityIsEnding) override;

protected:
	virtual void SharedInitAndApply() override;

	UPROPERTY(Replicated)
	FVector WorldDirection;

	UPROPERTY(Replicated)
	float Strength;

	UPROPERTY(Replicated)
	float Duration;

	UPROPERTY(Replicated)
	bool bIsAdditive;

	UPROPERTY(Replicated)
	FName CurveFloatName;

	UPROPERTY(Replicated)
	FGameplayTag MontageTag;

	UPROPERTY(Replicated)
	bool bEnableGravity;
};
