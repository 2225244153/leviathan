// Fill out your copyright notice in the Description page of Project Settings.


#include "InputActionManageComponent.h"

#include "EnhancedInputComponent.h"
#include "GHInputAction.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UInputActionManageComponent::UInputActionManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInputActionManageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ENetRole::ROLE_AutonomousProxy)
	{
		Character = Cast<ACharacter>(GetOwner());

		PlayerController = Cast<APlayerController>(Character->GetController());
		InitInputAction(PlayerController);
	}
}

void UInputActionManageComponent::InitInputAction(APlayerController* InController)
{
	check(InputActionBindings);
	for (auto& InputActionData : InputActionBindings->InputActionBindingList)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(
			InController->InputComponent))
		{
			InputActionData.Init(Character, EnhancedInputComponent);
		}
	}
}
