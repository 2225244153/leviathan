#pragma once

#include "CoreMinimal.h"
#include "GHAnimSkillAbility.h"
#include "Leviathan/GHComponents/SkillKnockComponent.h"
#include "UObject/Object.h"
#include "GHKnockAction.generated.h"

class USkillKnockComponent;
/**
 * 
 */
UCLASS()
class LEVIATHAN_API UGHKnockAction : public UGHAnimSkillAbility
{
	GENERATED_BODY()

public:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	
	USkillKnockComponent* GetKnockComponent();

	UPROPERTY()
	class UGHAbilityTaskApplyRootMotionConstantForce* ApplyRootMotionConstantForceTask;

	UPROPERTY()
	UCurveFloat* AnimCurveForStrength;

	UPROPERTY()
	FVector Direction;

	UPROPERTY()
	USkillKnockComponent* KnockComponent;
};
