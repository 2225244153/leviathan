// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTargetComponent.h"

#include "Leviathan/GHGameFrameWork/GHBasePlayer.h"
#include "Leviathan/GHManagers/GHCharacterMgr.h"


// Sets default values for this component's properties
UBattleTargetComponent::UBattleTargetComponent() : bFindBattleTarget(false), BattleTarget(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBattleTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AGHBaseCharacter>(GetOwner());
}


// Called every frame
void UBattleTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bFindBattleTarget)
	{
		FindBattleTarget();
	}
	else
	{
		ResetBattleTarget();
	}
}

AGHBaseCharacter* UBattleTargetComponent::GetBattleTarget()
{
	return BattleTarget;
}

void UBattleTargetComponent::ResetBattleTarget()
{
	BattleTarget = nullptr;
}

void UBattleTargetComponent::FindBattleTarget()
{
	if (!bFindBattleTarget)
	{
		ResetBattleTarget();
		return;
	}

	TArray<AGHBasePlayer*> allPlayers;
	UGHCharacterMgr::Get()->GetAllPlayers(allPlayers);
	for (auto& player : allPlayers)
	{
		if (player->IsValidLowLevelFast())
		{
			continue;
		}

		//todo
	}
}

