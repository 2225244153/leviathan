// Copyright Relink Games, Inc. All Rights Reserved. 


#include "GHAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "Leviathan/AbilitySystem/Abilities/GHGameplayAbility.h"
#include "Leviathan/AbilitySystem/Attribute/GHAttributeSetBase.h"

#define LOCTEXT_NAMESPACE "GHAbilitySystemComponent"
DEFINE_LOG_CATEGORY(LogGHAbilitySystemComponent)

// Sets default values for this component's properties
UGHAbilitySystemComponent::UGHAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGHAbilitySystemComponent::K2_CancelAbilityHandle(const FGameplayAbilitySpecHandle AbilitySpecHandle)
{
	CancelAbilityHandle(AbilitySpecHandle);
}

void UGHAbilitySystemComponent::K2_CancelAbilities(const FGameplayTagContainer& WithTags,
                                                   const FGameplayTagContainer& WithOutTags, UGameplayAbility* Ignore)
{
	CancelAbilities(&WithTags, &WithOutTags, Ignore);
}

void UGHAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

FActiveGameplayEffect* UGHAbilitySystemComponent::GetActiveGameplayEffect(const FActiveGameplayEffectHandle& Handle)
{
	return ActiveGameplayEffects.GetActiveGameplayEffect(Handle);
}

bool UGHAbilitySystemComponent::AddAttributeSetDesc(UGHAttributeSetDesc* AttributeSetDesc)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return false;
	}

	if (AttributeSetDesc == nullptr)
	{
		return false;
	}

	int _index = 0;
	for (const TSubclassOf<UGHAttributeSetBase>& att_base_class : AttributeSetDesc->AttributeSet)
	{
		if (!att_base_class)
		{
			FMessageLog AttributeSetMessageLog("AttributeSetMessageLog");
			FText text = FText::Format(LOCTEXT("AttributeSetContainInvalidData", "ASD 中包含无效的数据{0}= [第{1}个]"),
			                           FText::FromString(*AttributeSetDesc->GetFullName()),
			                           FText::AsNumber((int32)_index));

			AttributeSetMessageLog.Error(text);
			AttributeSetMessageLog.Open(EMessageSeverity::Error);
			continue;
		}

		UGHAttributeSetBase* att_base_set = NewObject<UGHAttributeSetBase>(GetOwner(), att_base_class);
		check(att_base_set);

		if (GetAttributeSet(att_base_class) != nullptr)
		{
			continue;
		}

		AddSpawnedAttribute(att_base_set);
		_index++;
	}

	ForceReplication();
	return true;
}

void UGHAbilitySystemComponent::InitAttributeSetDefaultValue(UGHAttributeSetDesc* AttributeSetDesc)
{
	// 初始化属性数值
	UGameplayEffect* GE = NewObject<UGameplayEffect>(this);
	check(GE);

	GE->DurationPolicy = EGameplayEffectDurationType::Instant;
	for (const TPair<FGameplayAttribute, float>& pair : AttributeSetDesc->AttributeDefault)
	{
		if (HasAttributeSetForAttribute(pair.Key))
		{
			FGameplayModifierInfo DefaultAttModifierInfo;

			DefaultAttModifierInfo.Attribute = pair.Key;
			DefaultAttModifierInfo.ModifierOp = EGameplayModOp::Override;

			FScalableFloat GEFloatScalar = pair.Value;
			DefaultAttModifierInfo.ModifierMagnitude = GEFloatScalar;
			GE->Modifiers.Add(DefaultAttModifierInfo);
		}
	}

	ApplyGameplayEffectToSelf(GE, 1.f, MakeEffectContext());
}

void UGHAbilitySystemComponent::ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag,
                                                        const FGameplayCueParameters& GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(
		GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Executed, GameplayCueParameters);
}

void UGHAbilitySystemComponent::K2_ExecuteGameplayCue(const FGameplayTag GameplayCueTag,
                                                      const FGameplayCueParameters& GameplayCueParameters)
{
	ExecuteGameplayCue(GameplayCueTag, GameplayCueParameters);
}

void UGHAbilitySystemComponent::K2_AddGameplayCue(const FGameplayTag GameplayCueTag,
                                                  const FGameplayCueParameters& GameplayCueParameters)
{
	AddGameplayCue(GameplayCueTag, GameplayCueParameters);
}

void UGHAbilitySystemComponent::K2_RemoveGameplayCue(const FGameplayTag GameplayCueTag)
{
	RemoveGameplayCue(GameplayCueTag);
}

UGameplayAbility* UGHAbilitySystemComponent::FindGameplayAbilityByHandle(const FGameplayAbilitySpecHandle& handle)
{
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Handle == handle)
		{
			UGameplayAbility* GameplayAbility = Spec.GetPrimaryInstance();
			return GameplayAbility ? GameplayAbility : Spec.Ability.Get();
		}
	}
	return nullptr;
}

bool UGHAbilitySystemComponent::InitAttributeSet()
{
	if (!AddAttributeSetDesc(AttributeSetDefault))
	{
		return false;
	}

	InitAttributeSetDefaultValue(AttributeSetDefault);
	return true;
}

bool UGHAbilitySystemComponent::InitAbility()
{
	if (AbilityInfoListAsset == nullptr)
	{
		return false;
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		// 权威端会give所有默认技能类
		for (auto& AbilityInfo : AbilityInfoListAsset->AbilityInfoList)
		{
			if (AbilityInfo.AbilityClass == nullptr)
			{
				continue;
			}

			if (!Give(AbilityInfo.AbilityClass, AbilityInfo.Level, 1))
			{
				continue;
			}
		}
	}

	return false;
}

bool UGHAbilitySystemComponent::Give(TSubclassOf<UGHGameplayAbility> AbilityClass, int32 SkillLevel, int32 InputID)
{
	check(AbilityClass);
	FGameplayAbilitySpec AbilitySpec = BuildAbilitySpecFromClass(
		AbilityClass, SkillLevel, InputID);
	FGameplayAbilitySpecHandle SpecHandle = GiveAbility(AbilitySpec);
	
	if(!SpecHandle.IsValid())
	{
		return false;
	}
		
	return true;
}

void UGHAbilitySystemComponent::AddLooseTag(const FGameplayTag& tag)
{
	AddLooseGameplayTag(tag);
}

void UGHAbilitySystemComponent::RemoveLooseTag(const FGameplayTag& tag)
{
	RemoveLooseGameplayTag(tag);
}

#undef LOCTEXT_NAMESPACE
