// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillKnockComponent.h"

#include "Leviathan/GHGameFrameWork/GHBaseCharacter.h"
#include "Leviathan/Log/GHLog.h"


// Sets default values for this component's properties
USkillKnockComponent::USkillKnockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillKnockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USkillKnockComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillKnockComponent::ActivateKnock(const FKnockInfo& knock_info, const FVector& dir, const float& strength)
{
	ForceRotateAndMove(knock_info, dir, strength);
	CurrentKnockInfo = knock_info;
	CurrentKnockInfo.KnockForceMoveInfo.Strength = strength;
	CurrentKnockInfo.KnockForceMoveInfo.Direction = dir;
	CurrentKnockInfo.KnockForceMoveInfo.EnableGravity = false;
	IAbilitySystemInterface* ability_system_interface = Cast<IAbilitySystemInterface>(GetOwner());

	if (!DefaultKnockAbility)
	{
		ensureMsgf(false, TEXT("Unable To Find Knock Ability"));
		return;
	}
	ability_system_interface->GetAbilitySystemComponent()->TryActivateAbilityByClass(
		DefaultKnockAbility);
}

void USkillKnockComponent::ForceRotateAndMove(const FKnockInfo& knock_info, const FVector& dir, const float& strength)
{
	{
		if (dir.Length() > 0)
		{
			if (knock_info.bForceChangeDirection)
			{
				FRotator rotator = (dir * -1.f).Rotation();
				//把Roll和Pitch归零，否则角色旋转会受到影响
				rotator.Roll = 0.f;
				rotator.Pitch = 0.f;

				if (AGHBaseCharacter* character = GetOwner<AGHBaseCharacter>())
				{
					character->SetActorRotation(rotator);
				}
			}

			if (knock_info.bForceMove)
			{
				CurrentKnockInfo.KnockForceMoveInfo.Strength = 10;
			}
		}
	}
}

void USkillKnockComponent::HandleSkillKnock()
{
}
