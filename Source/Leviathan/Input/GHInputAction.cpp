// Fill out your copyright notice in the Description page of Project Settings.


#include "GHInputAction.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"

void FGHInputActionBinding::Init(ACharacter* Character, UEnhancedInputComponent* InEnhancedInputComponent)
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Character);
	check(AbilitySystemInterface);

	UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
	check(AbilitySystemComponent);

	BindingHandle = 0;

	if (!EnhancedInputComponent)
	{
		EnhancedInputComponent = InEnhancedInputComponent;
	}

	if (!GHInputAction)
	{
		GHInputAction = NewObject<UGHInputAction>(EnhancedInputComponent, InputActionClass);
	}
	check(GHInputAction);

	GHInputAction->Init(Character);
	if (InputTag.IsValid())
	{
		GHInputAction->InputTag = InputTag;
	}
	BindInputAction();
	if (AbilitySystemComponent->HasMatchingGameplayTag(DisableInputTag))
	{
		if (DisableInputNeedRemoveBinding)
		{
			RemoveBinding();
		}
		GHInputAction->DisableInput = true;
	}
}

void FGHInputActionBinding::BindInputAction()
{
	if (BindingHandle == 0)
	{
		BindingHandle = EnhancedInputComponent->BindAction(InputAction, TriggerEvent, GHInputAction,
		                                                   &UGHInputAction::OnInputAction).GetHandle();
	}
}

void FGHInputActionBinding::RemoveBinding()
{
	EnhancedInputComponent->RemoveBindingByHandle(BindingHandle);
	BindingHandle = 0;
}

void UGHInputAction::OnInputAction(const FInputActionValue& InputActionValue)
{
}

void UGHInputAction::Init(ACharacter* InCharacter)
{
	Character = InCharacter;
}
