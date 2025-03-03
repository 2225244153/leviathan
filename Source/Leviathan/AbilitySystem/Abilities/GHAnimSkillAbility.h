#pragma once

#include "CoreMinimal.h"
#include "GHGameplayAbility.h"
#include "GHAnimSkillAbility.generated.h"

class AGHTargetActor;
struct FGHAnimMontageInfo;
class UAbilityTask_WaitTargetData;
class UGHAbilityTaskPlayMontageAndWait;

USTRUCT(BlueprintType)
struct FGHMontageStartSection
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StartSection")
	FGameplayTag Tag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "StartSection")
	FName SectionName;
};

USTRUCT(BlueprintType)
struct FGHAnimSkillMontagePlayer
{
	GENERATED_BODY()

	/**
	 * @brief 动作的蒙太奇tag
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
	FGameplayTag ActionMontageTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action|Montage")
	float PlayMontageRate = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action|Montage")
	float PlayMontageStartTimeSeconds = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action|Montage")
	bool StopWhenAbilityEnd = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action|Montage")
	float RootMotionTranslationScale = 1.f;

	UPROPERTY(BlueprintReadWrite, Category = "Action|Montage")
	UGHAbilityTaskPlayMontageAndWait* ActionMontagePlayer;
};

/**
 * Anim Skill Ability.
 * 
 */
UCLASS()
class LEVIATHAN_API UGHAnimSkillAbility : public UGHGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	FGHAnimMontageInfo* GetAnimMontageInfo(const FGameplayTag& montage_tag);

	virtual void ActiveTargetActor(TSubclassOf<AGHTargetActor> target_actor_class);
	void SpawnAndWaitTargetData(TEnumAsByte<EGameplayTargetingConfirmation::Type> confirmation_type, TSubclassOf<AGHTargetActor> target_actor_class);

	UFUNCTION()
	virtual void OnReceiveTargetData(const FGameplayAbilityTargetDataHandle& data);

	UFUNCTION()
	virtual void OnCancelTargetData(const FGameplayAbilityTargetDataHandle& data);

	UFUNCTION()
	virtual void OnEventReceived(FGameplayEventData payload);

	bool InitAnimMontagePlayer(FGHAnimSkillMontagePlayer& anim_skill_montage_player);
	void EndAnimMontagePlayer(FGHAnimSkillMontagePlayer& anim_skill_montage_player);

	UFUNCTION(BlueprintNativeEvent)
	void OnActionBlendOut();
	UFUNCTION(BlueprintNativeEvent)
	void OnActionCompleted();
	UFUNCTION(BlueprintNativeEvent)
	void OnActionInterrupted();
	UFUNCTION(BlueprintNativeEvent)
	void OnActionCancelled();
	UFUNCTION(BlueprintNativeEvent)
	float GetRootMotionScale();

protected:
	void WaitHandleDamage();
	
	/**
	 * @brief 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim")
	FGHAnimSkillMontagePlayer AnimBeforeMontagePlayer;

	/**
	 * @brief 当前的TargetActor
	 */
	UPROPERTY(BlueprintReadWrite, Transient, Category = "Template Data")
	AGHTargetActor* CurrentTargetActor;

	/**
	 * @brief  
	 */
	UPROPERTY(BlueprintReadWrite, Transient, Category = "Template Data")
	UAbilityTask_WaitTargetData* CurrentTargetDataTask;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TMap<FGameplayTag, FGHGameplayEffectContainer> EffectContainerMap;

	/**
	 * @brief 当前正在播放的蒙太奇
	 */
	FGameplayTag CurrentActionMontageTag;

	FGameplayEventData PayLoad;

	FGHAnimSkillMontagePlayer* CurrentMontagePlayer;
};
