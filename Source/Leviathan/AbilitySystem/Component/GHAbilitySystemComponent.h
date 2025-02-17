// Copyright Relink Games, Inc. All Rights Reserved. 

#pragma once
#include "../Abilities/GHAbilityInfo.h"
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GHAbilitySystemComponent.generated.h"

USTRUCT(BlueprintType)
struct FInputActionAbilityMap
{
	GENERATED_BODY()

	//技能Map
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InputAction")
	TMap<FGameplayTag, TSubclassOf<UGHGameplayAbility>> InputActionMap;
};

DECLARE_LOG_CATEGORY_EXTERN(LogGHAbilitySystemComponent, Log, All)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEVIATHAN_API UGHAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGHAbilitySystemComponent();

	UFUNCTION(BlueprintCallable, meta=(DisplayName = "CancelAbilityHandle"))
	void K2_CancelAbilityHandle(const FGameplayAbilitySpecHandle AbilitySpecHandle);

	UFUNCTION(BlueprintCallable, meta=(DisplayName = "CancelAbilies"))
	void K2_CancelAbilities(const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithOutTags,
	                        UGameplayAbility* Ignore = nullptr);

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor);

	/**
	 * @brief 
	 * @param Handle 
	 * @return 
	 */
	FActiveGameplayEffect* GetActiveGameplayEffect(const FActiveGameplayEffectHandle& Handle);

	// Attribute
	bool AddAttributeSetDesc(class UGHAttributeSetDesc* AttributeSetDesc);
	void InitAttributeSetDefaultValue(class UGHAttributeSetDesc* AttributeSetDesc);

	/**
	 * @brief 
	 * @param GameplayCueTag 
	 * @param GameplayCueParameters 
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag,
	                             const FGameplayCueParameters& GameplayCueParameters);

	UFUNCTION(BlueprintCallable, Category = "GameplayCue", DisplayName = "ExecuteGameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void K2_ExecuteGameplayCue(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

	/**
	 * @brief 
	 * @param GameplayCueTag 
	 * @param GameplayCueParameters 
	 */
	UFUNCTION(BlueprintCallable, Category = "GameplayCue", DisplayName = "AddGameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void K2_AddGameplayCue(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

	UFUNCTION(BlueprintCallable, Category = "GameplayCue", DisplayName = "RemoveGameplayCue",
		Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void K2_RemoveGameplayCue(const FGameplayTag GameplayCueTag);

	UFUNCTION(BlueprintCallable, Category = "Tag")
	void AddLooseTag(const FGameplayTag& tag);

	UFUNCTION(BlueprintCallable, Category = "Tag")
	void RemoveLooseTag(const FGameplayTag& tag);

	UFUNCTION(BlueprintCallable)
	UGameplayAbility* FindGameplayAbilityByHandle(const FGameplayAbilitySpecHandle& handle);

	UFUNCTION(BlueprintCallable)
	bool InitAttributeSet();

	/*
	 * 初始化技能
	 */
	bool InitAbility();

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGHGameplayAbility> GetAbilityByInputTag(FGameplayTag InputTag);

private:
	bool Give(TSubclassOf<UGHGameplayAbility> AbilityClass, int32 SkillLevel, int32 InputID);

	//某些逻辑需要延迟到AbilitySystemComponent初始化好以后，这里提供时机
	void OnAbilitySystemComponentReady();

public:
	/*
	 * 默认技能
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UGHAbilityInfoList> AbilityInfoListAsset;

	/**
	 * @brief 属性集合描述
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	TObjectPtr<UGHAttributeSetDesc> AttributeSetDefault;

	// 表征技能系统
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="")
	FInputActionAbilityMap AbilityMap;
};
