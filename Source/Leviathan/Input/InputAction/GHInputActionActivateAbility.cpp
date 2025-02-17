// Fill out your copyright notice in the Description page of Project Settings.


#include "GHInputActionActivateAbility.h"
#include "../../AbilitySystem/Component/GHAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

void UGHInputActionActivateAbility::OnInputAction(const FInputActionValue& input_action_value)
{
	Super::OnInputAction(input_action_value);

	TSubclassOf<UGameplayAbility> GameplayAbility = AbilitySystemComponent->GetAbilityByInputTag(InputTag);
	bool bIsSuccess = AbilitySystemComponent->TryActivateAbilityByClass(GameplayAbility);
	if (!bIsSuccess)
	{
		return;
	}
}

void UGHInputActionActivateAbility::Init(ACharacter* InCharacter)
{
	Super::Init(InCharacter);

	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(InCharacter);
	if (AbilitySystemInterface)
	{
		AbilitySystemComponent = Cast<UGHAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
	}
}
