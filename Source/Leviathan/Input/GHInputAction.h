// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "GHInputAction.generated.h"

USTRUCT(BlueprintType)
struct FGHInputActionBinding
{
	GENERATED_BODY()

public:
	/**
	 * @brief IA
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InputAction;

	/**
	 * @brief 输入回调逻辑
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	TSubclassOf<UGHInputAction> InputActionClass;

	/**
	 * @brief 触发方式
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	ETriggerEvent TriggerEvent;

	/**
	 * @brief 输入Tag，当IA激活时会激活相应Tag
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	FGameplayTag InputTag;

	/**
	 * @brief 屏蔽该输入的Tag
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DisableInput")
	FGameplayTag DisableInputTag;

	/**
	 * @brief 是否需要移除绑定
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DisableInput")
	bool DisableInputNeedRemoveBinding = false;

	/**
	 * @brief 保存的实例
	 */
	UPROPERTY()
	class UGHInputAction* GHInputAction;

	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;

	int32 BindingHandle = 0;

	void Init(ACharacter* Character, UEnhancedInputComponent* EnhancedInputComponent);

	void BindInputAction();

	void RemoveBinding();
};

UCLASS()
class UGHInputActionBindingList : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGHInputActionBinding> InputActionBindingList;
};

/**
 * 
 */
UCLASS()
class LEVIATHAN_API UGHInputAction : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief 输入回调
	 * @param InputActionValue 
	 */
	UFUNCTION()
	virtual void OnInputAction(const FInputActionValue& InputActionValue);

	/**
	 * @brief 初始化
	 * @param input_action
	 * @param InCharacter 
	 */
	virtual void Init(ACharacter* InCharacter);

	UPROPERTY()
	FGameplayTag InputTag;

	UPROPERTY()
	bool DisableInput = false;

	UPROPERTY()
	ACharacter* Character;
};
