

#include "GHAttributeSetBase.h"

#include "Leviathan/AbilitySystem/Component/GHAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UWorld* UGHAttributeSetBase::GetWorld() const
{
	const UObject* outer = GetOuter();
	check(outer);

	return outer->GetWorld();
}

UGHAbilitySystemComponent* UGHAttributeSetBase::GetAbilitySystemComponent() const
{
	return Cast<UGHAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

void UGHAttributeSetBase::ScaleAttributeWithMax(const FGameplayAttributeData& attribute_data,
                                                    const FGameplayAttribute& attribute, float new_max_value,
                                                    const FGameplayAttributeData& max_attribute_data) const
{
	const float current_max_value = max_attribute_data.GetCurrentValue();

	if (!FMath::IsNearlyEqual(current_max_value, new_max_value))
	{
		UAbilitySystemComponent* asc = GetOwningAbilitySystemComponent();
		if (asc == nullptr)
		{
			return;
		}

		const float current_value = attribute_data.GetCurrentValue();
		const float delta = (current_max_value > 0.f)
			                    ? current_value * new_max_value / current_max_value - current_value
			                    : new_max_value;

		// 调用下面的方法是否安全？ 
		asc->ApplyModToAttributeUnsafe(attribute, EGameplayModOp::Additive, delta);
	}
}

void UGHAttributeSetBase::ScaleAttributeWithMax(const FGameplayAttributeData& attribute_data,
                                                    const FGameplayAttribute& attribute, float old_value,
                                                    float new_value) const
{
	//LOG_INFO("old_value:%f, new_value:%f"), old_value, new_value);

	if (!FMath::IsNearlyEqual(old_value, new_value))
	{
		const float current_value = attribute_data.GetCurrentValue();
		const float delta = (old_value > 0.f) ? current_value * new_value / old_value - current_value : new_value;

		UAbilitySystemComponent* asc = GetOwningAbilitySystemComponent();
		if (asc == nullptr)
		{
			return;
		}

		asc->ApplyModToAttributeUnsafe(attribute, EGameplayModOp::Additive, delta);
	}
}

float UGHAttributeSetBase::ClampAttribute(FGameplayAttributeData& attribute_data, float new_value, float min_value,
                                              float max_value) const
{
	const float current_value = attribute_data.GetCurrentValue();
	if (!FMath::IsNearlyEqual(new_value, current_value))
	{
		new_value = FMath::Clamp(new_value, min_value, max_value);
		attribute_data.SetBaseValue(new_value);
	}

	return new_value;
}

void UGHHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGHHealthAttributeSet, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UGHHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);
}

void UGHHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UGHHealthAttributeSet::OnRepHealth(const FGameplayAttributeData& old_value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGHHealthAttributeSet, Health, old_value);
}

void UGHHealthAttributeSet::OnRepMaxHealth(const FGameplayAttributeData& old_value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGHHealthAttributeSet, MaxHealth, old_value);
}

void UGHAttackAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGHAttackAttributeSet, Attack, COND_None, REPNOTIFY_OnChanged);
}

void UGHAttackAttributeSet::OnRepAttack(const FGameplayAttributeData& old_value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGHAttackAttributeSet, Attack, old_value);
}
