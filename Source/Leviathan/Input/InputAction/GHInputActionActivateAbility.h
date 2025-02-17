// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Leviathan/Input/GHInputAction.h"
#include "GHInputActionActivateAbility.generated.h"

class UGHAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class LEVIATHAN_API UGHInputActionActivateAbility : public UGHInputAction
{
	GENERATED_BODY()
public:
	virtual void OnInputAction(const FInputActionValue& input_action_value) override;

	virtual void Init(ACharacter* InCharacter) override;

private:
	UPROPERTY()
	UGHAbilitySystemComponent* AbilitySystemComponent;
};
