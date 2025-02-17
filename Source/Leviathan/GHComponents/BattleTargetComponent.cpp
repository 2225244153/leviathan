// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTargetComponent.h"

#include "Leviathan/GameDefine.h"
#include "Leviathan/GHGameFrameWork/GHBasePlayer.h"
#include "Leviathan/GHManagers/GHCharacterMgr.h"
#include "Leviathan/GHUtils/GHCommonUtils.h"


// Sets default values for this component's properties
UBattleTargetComponent::UBattleTargetComponent() : bFindBattleTarget(true), BattleTarget(nullptr)
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
	//目标不合法才需要重新查找目标，demo暂不支持切换目标的逻辑
	if (CheckTargetValid())
	{
		return;
	}

	TArray<AGHBasePlayer*> allPlayers;
	UGHCharacterMgr::Get()->GetAllPlayers(allPlayers);
	AGHBasePlayer* tempTarget = nullptr;
	for (auto& player : allPlayers)
	{
		if (!player->IsValidLowLevelFast())
		{
			continue;
		}
		
		float dis = UGHCommonUtils::CalcDistance(Owner, player);
		if (dis > AIFindTargetWarnMaxDistance)
		{
			continue;
		}

		float angle = UGHCommonUtils::Calc2DAngleByForward(Owner, player);
		if (angle > (AIFindTargetAngle / 2))
		{
			continue;
		}
		//todo 选到目标就直接退出，暂不考虑择优选择
		tempTarget = player;
		break;
	}
	
	BattleTarget = tempTarget;
	if (BattleTarget != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("hsp------------- find target: %s"), *BattleTarget->GetName());
	}
}

bool UBattleTargetComponent::CheckTargetValid()
{
	if (BattleTarget == nullptr)
	{
		return false;
	}

	float dis = UGHCommonUtils::CalcDistance(Owner, BattleTarget);
	return dis < AIPursueMaxDistance;
}

