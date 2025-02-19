// Copyright Relink Games, Inc. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GHGameplayAbility.generated.h"

class UGHAbilitySystemComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogGHGameplayAbility, Log, All)

/**
 * UGHGameplayAbility
 * Ability基础类
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class"))
class LEVIATHAN_API UGHGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGHGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	/**
	 * @brief 通过ActorInfo获取ASC
	 * @return 
	 */
	UGHAbilitySystemComponent* GetAbilitySystemComponentFromActorInfo() const;

public:
	/**
	 * @brief 当Ability激活失败后的事件
	 * @param FailureReason 失败的原因
	 */
	void OnAbilityFailedToActivate(const FGameplayTagContainer& FailureReason) const;

public:
	/**
	 * @brief 当目标赋予Ability时的事件
	 * @param ActorInfo 
	 * @param AbilitySpec 
	 */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	                           const FGameplayAbilitySpec& AbilitySpec) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	

protected:
	/**
	 * @brief BP 激活失败时通知
	 * @param failure_reason 
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptOnAbilityFailedToActivate(const FGameplayTagContainer& failure_reason) const;

	void EndCurrentAbility(bool is_replicate_end_ability, bool is_was_cancelled);

private:
	FDelegateHandle HandleEventApplyEffectContainerDelegate;
	FDelegateHandle HandleEventRemoveEffectContainerDelegate;
};

/**
 * 技能描述文件
 */
USTRUCT(BlueprintType)
struct LEVIATHAN_API FGHAbilitySpec
{
	GENERATED_BODY()

	/**
	 * @brief 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	TSubclassOf<UGHGameplayAbility> AbilityClass;

	/**
	 * @brief 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	int Level = 1;
};


/** 
 *
 * 
 */
UCLASS(BlueprintType)
class LEVIATHAN_API UGHAbilityContainer : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability Container")
	TArray<FGHAbilitySpec> AbilityList;
};
