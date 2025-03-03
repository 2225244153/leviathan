
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

USTRUCT(BlueprintType)
struct FGHAbilityDesc
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AbilityDesc")
	int32 SkillID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AbilityDesc")
	FGameplayAbilitySpecHandle AbilitySpecHandle;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEVIATHAN_API UGHAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Sets default values for this component's properties
	UGHAbilitySystemComponent();

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
	 * @return 返回"可用的”SkillID列表
	 */
	UFUNCTION(BlueprintPure)
	TArray<int32> GetUsableAbilities();

	UFUNCTION(BlueprintCallable)
	bool TryActivateSkill(int32 SkillID);

	UFUNCTION(BlueprintCallable)
	bool InitAttributeSet();

	/*
	 * 初始化技能
	 */
	bool InitAbility();

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGHGameplayAbility> GetAbilityByInputTag(FGameplayTag InputTag);

private:
	bool Give(int32 SkillID, int32 SkillLevel, int32 InputID);

	//某些逻辑需要延迟到AbilitySystemComponent初始化好以后，这里提供时机
	void OnAbilitySystemComponentReady();

	int32 FindSkillIDByAbilityHandle(const FGameplayAbilitySpecHandle& AbilitySpecHandle);

public:
	/*
	 * 默认技能
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UGHAbilityInfoList>> AbilityInfoListAssets;
	
	/**
	 * @brief 属性集合描述
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attribute")
	TObjectPtr<UGHAttributeSetDesc> AttributeSetDefault;

	/*
	 * 输入与技能对应的部分
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="")
	FInputActionAbilityMap AbilityMap;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<FGHAbilityDesc> AbilityDescs;
};
