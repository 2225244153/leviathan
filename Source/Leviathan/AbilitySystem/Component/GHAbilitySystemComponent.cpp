

#include "GHAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Leviathan/AbilitySystem/Abilities/GHGameplayAbility.h"
#include "Leviathan/AbilitySystem/Attribute/GHAttributeSetBase.h"
#include "Leviathan/GHUtils/SkillUtils.h"
#include "Leviathan/Log/GHLog.h"
#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "GHAbilitySystemComponent"

void UGHAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGHAbilitySystemComponent, AbilityDescs);
}

// Sets default values for this component's properties
UGHAbilitySystemComponent::UGHAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
		else
		{
			ensureMsgf(false,TEXT("未正确添加此属性所对应的属性集"));
		}
	}

	ApplyGameplayEffectToSelf(GE, 1.f, MakeEffectContext());
}

TArray<int32> UGHAbilitySystemComponent::GetUsableAbilities()
{
	TArray<FGameplayAbilitySpec> InActivatableAbilities = GetActivatableAbilities();

	TArray<int32> SkillIDs;
	for (const FGameplayAbilitySpec& Spec : InActivatableAbilities)
	{
		UGameplayAbility* AbilityInstance = Spec.GetPrimaryInstance();
		if (AbilityInstance && AbilityInstance->CanActivateAbility(Spec.Handle, AbilityActorInfo.Get(), nullptr,
		                                                           nullptr, nullptr))
		{
			int32 SkillID = FindSkillIDByAbilityHandle(Spec.Handle);
			if (SkillID != -1)
			{
				SkillIDs.Add(SkillID);
			}
		}
	}

	return SkillIDs;
}

bool UGHAbilitySystemComponent::TryActivateSkill(int32 SkillID)
{
	FSkillData SkillData = USkillUtils::GetSkillDataBySkillID(SkillID);
	if (!SkillData.IsValid())
	{
		return false;
	}

	return TryActivateAbilityByClass(SkillData.Ability);
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
	if (AbilityInfoListAssets.Num() == 0)
	{
		return false;
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		for (const auto& AbilityInfoListAsset : AbilityInfoListAssets)
		{
			// 权威端会give所有默认技能类
			for (auto& AbilityInfo : AbilityInfoListAsset->AbilityInfoList)
			{
				if (!Give(AbilityInfo.SkillID, AbilityInfo.Level, 1))
				{
					LOG_ERROR("技能赋予失败 SkillID:%d", AbilityInfo.SkillID);
				}
			}
		}
	}

	return false;
}

TSubclassOf<UGHGameplayAbility> UGHAbilitySystemComponent::GetAbilityByInputTag(FGameplayTag InputTag)
{
	TSubclassOf<UGHGameplayAbility>* GameplayAbilityClass = AbilityMap.InputActionMap.Find(InputTag);
	if (!GameplayAbilityClass)
	{
		return nullptr;
	}

	return *GameplayAbilityClass;
}


bool UGHAbilitySystemComponent::Give(int32 SkillID, int32 SkillLevel, int32 InputID)
{
	FSkillData SkillData = USkillUtils::GetSkillDataBySkillID(SkillID);
	if (!SkillData.IsValid())
	{
		return false;
	}

	FGHAbilityDesc AbilityDesc;
	FGameplayAbilitySpec AbilitySpec = BuildAbilitySpecFromClass(
		SkillData.Ability, SkillLevel, InputID);
	AbilityDesc.AbilitySpecHandle = GiveAbility(AbilitySpec);
	AbilityDesc.SkillID = SkillID;
	AbilityDescs.Emplace(AbilityDesc);
	if (!AbilityDesc.AbilitySpecHandle.IsValid())
	{
		return false;
	}

	return true;
}

void UGHAbilitySystemComponent::OnAbilitySystemComponentReady()
{
}

int32 UGHAbilitySystemComponent::FindSkillIDByAbilityHandle(const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	FGHAbilityDesc* AbilityDesc = AbilityDescs.FindByPredicate([AbilitySpecHandle](const FGHAbilityDesc& AbilityDesc)
	{
		return AbilityDesc.AbilitySpecHandle == AbilitySpecHandle;
	});

	if (AbilityDesc)
	{
		return AbilityDesc->SkillID;
	}

	return -1;
}

#undef LOCTEXT_NAMESPACE
