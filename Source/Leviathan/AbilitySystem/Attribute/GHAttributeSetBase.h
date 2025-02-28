#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GHAttributeSetBase.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define ATTRIBUTE_CLAMP(PropertyName, MinValue, MaxValue) \
{ 	float value = FMath::Clamp(Get##PropertyName(), (MinValue), (MaxValue));		\
if (!FMath::IsNearlyEqual(value, Get##PropertyName()))	Set##PropertyName(value); }

/**
 *  属性集基础类.
 */
UCLASS(Blueprintable, BlueprintType)
class LEVIATHAN_API UGHAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	/**
	 * @brief 
	 * @return 
	 */
	virtual UWorld* GetWorld() const override;

	/**
	 * @brief 
	 * @return 
	 */
	class UGHAbilitySystemComponent* GetAbilitySystemComponent() const;


	/**
	 * @brief 调整最大值时根据当前值与最大之的比例进行调整。
	 * @param attribute_data 
	 * @param attribute 
	 * @param new_max_value 
	 * @param max_attribute_data 
	 */
	void ScaleAttributeWithMax(const FGameplayAttributeData& attribute_data, const FGameplayAttribute& attribute,
	                           float new_max_value, const FGameplayAttributeData& max_attribute_data) const;

	/**
	 * @brief 调整最大值时根据当前值与最大之的比例进行调整。
	 * @param attribute_data 
	 * @param attribute 
	 * @param old_value 
	 * @param new_value 
	 */
	void ScaleAttributeWithMax(const FGameplayAttributeData& attribute_data, const FGameplayAttribute& attribute,
	                           float old_value, float new_value) const;

	/**
	 * @brief 限制属性值的范围
	 * @param attribute_data 
	 * @param new_value 
	 * @param min_value 
	 * @param max_value 
	 * @return 
	 */
	float ClampAttribute(FGameplayAttributeData& attribute_data, float new_value, float min_value,
	                     float max_value) const;
};

/**
 * 属性集合资产
 * 
 */
UCLASS(BlueprintType)
class UGHAttributeSetDesc : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief 属性集合
	 */
	UPROPERTY(EditDefaultsOnly, Category= "AttributeSet")
	TArray<TSubclassOf<UGHAttributeSetBase>> AttributeSet;

	/**
	 * @brief 属性默认数值
	 */
	UPROPERTY(EditDefaultsOnly, Category= "AttributeSet")
	TMap<FGameplayAttribute, float> AttributeDefault;
};

UCLASS(BlueprintType)
class LEVIATHAN_API UGHHealthAttributeSet : public UGHAttributeSetBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	/**
     * @brief 当前血量
     */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRepHealth)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGHHealthAttributeSet, Health)

	/**
     * @brief 最大血量
     */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRepMaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGHHealthAttributeSet, MaxHealth)

	UFUNCTION()
	void OnRepHealth(const FGameplayAttributeData& old_value);
	
	UFUNCTION()
    void OnRepMaxHealth(const FGameplayAttributeData& old_value);
};

UCLASS(BlueprintType)
class LEVIATHAN_API UGHAttackAttributeSet : public UGHAttributeSetBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * @brief 当前攻击力
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRepAttack)
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UGHAttackAttributeSet, Attack)

	UFUNCTION()
	void OnRepAttack(const FGameplayAttributeData& old_value);
};
