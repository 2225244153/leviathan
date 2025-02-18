// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTargetComponent.h"

#include "Leviathan/GameDefine.h"
#include "Leviathan/GHGameFrameWork/GHBasePlayer.h"
#include "Leviathan/GHManagers/GHCharacterMgr.h"
#include "Leviathan/GHManagers/GHCoreDelegatesMgr.h"
#include "Leviathan/GHUtils/GHCommonUtils.h"


// Sets default values for this component's properties
UBattleTargetComponent::UBattleTargetComponent() : bFindBattleTarget(true), BattleTarget(nullptr), AlertTarget(nullptr), CurAlertValue(0)
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

	UpdateAlert(DeltaTime);
}

AGHBaseCharacter* UBattleTargetComponent::GetBattleTarget()
{
	return BattleTarget;
}

void UBattleTargetComponent::SetBattleTarget(AGHBaseCharacter* target)
{
	if (target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GH---BattleTargetComponent::SetBattleTarget: target is null!!!"));
		return;
	}
	BattleTarget = target;
	UGHCoreDelegatesMgr::OnBattleFindTarget.Broadcast(BattleTarget);
}

void UBattleTargetComponent::ResetBattleTarget()
{
	if (BattleTarget == nullptr)
	{
		return;
	}
	
	BattleTarget = nullptr;
	UGHCoreDelegatesMgr::OnBattleLoseTarget.Broadcast();
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
	AGHBasePlayer* tempalertTarget = nullptr;
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
			if (tempalertTarget == nullptr)
			{
				tempalertTarget = player;
			}
			continue;
		}
		//todo 选到目标就直接退出，暂不考虑择优选择
		tempTarget = player;
		tempalertTarget = nullptr;//选到目标清空警戒目标
		break;
	}

	//只要找到警戒目标就开始警戒，若找到目标则会停止警戒
	if (AlertTarget == nullptr && tempalertTarget != nullptr)
	{
		SetAlertTarget(tempalertTarget);
	}
	else if (AlertTarget != nullptr && tempalertTarget == nullptr)
	{
		SetAlertTarget(nullptr);
	}
	
	if ((BattleTarget == nullptr || BattleTarget != tempTarget) && tempTarget != nullptr)
	{
		FinishAlert();
		SetBattleTarget(tempTarget);
	}
	else if (BattleTarget != nullptr && tempTarget == nullptr)
	{
		ResetBattleTarget();
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

void UBattleTargetComponent::StartAlert()
{
	if (bAlert)
	{
		return;
	}
	bAlert = true;
	UGHCoreDelegatesMgr::OnStartAlert.ExecuteIfBound();
}

void UBattleTargetComponent::FinishAlert()
{
	if (!bAlert)
	{
		return;
	}
	if (CurAlertValue < 0 && AlertTarget == nullptr)
	{
		//丢失警戒目标退出的警戒状态
		UGHCoreDelegatesMgr::OnFinishAlert.ExecuteIfBound();
	}
	bAlert = false;
	CurAlertValue = 0;
	SetAlertTarget(nullptr);
}

void UBattleTargetComponent::UpdateAlert(float DeltaTime)
{
	if (!bAlert)
	{
		return;
	}

	if (CurAlertValue > MaxAlertValue || CurAlertValue < 0)
	{
		if (AlertTarget != nullptr)
		{
			SetBattleTarget(AlertTarget);
		}
		FinishAlert();
		return;
	}

	int32 factor = AlertTarget != nullptr ? 1 : -1;
	CurAlertValue += DeltaTime * AlertFloatRatePerSecond * factor;
}

void UBattleTargetComponent::SetAlertTarget(AGHBaseCharacter* alertTarget)
{
	if (alertTarget != nullptr)
	{
		StartAlert();
	}
	AlertTarget = alertTarget;
}

