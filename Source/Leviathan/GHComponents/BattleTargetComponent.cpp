// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTargetComponent.h"

#include "AIStateComponent.h"
#include "Leviathan/GameDefine.h"
#include "Leviathan/GHGameFrameWork/GHBaseMonster.h"
#include "Leviathan/GHGameFrameWork/GHBasePlayer.h"
#include "Leviathan/GHManagers/GHCharacterMgr.h"
#include "Leviathan/GHManagers/GHCoreDelegatesMgr.h"
#include "Leviathan/GHUtils/GHCommonUtils.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UBattleTargetComponent::UBattleTargetComponent()
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

	Owner = Cast<AGHBaseMonster>(GetOwner());

	EXECUTE_ON_SERVER
	{
		UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
		coreDelegatesMgr->OnAIStateChanged.AddUniqueDynamic(this, &UBattleTargetComponent::OnAIStateChanged);
		coreDelegatesMgr->OnCharacterHurt.AddUniqueDynamic(this, &UBattleTargetComponent::OnHurt);
	}
}

void UBattleTargetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EXECUTE_ON_SERVER
	{
		UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
		coreDelegatesMgr->OnAIStateChanged.RemoveDynamic(this, &UBattleTargetComponent::OnAIStateChanged);
		coreDelegatesMgr->OnCharacterHurt.RemoveDynamic(this, &UBattleTargetComponent::OnHurt);
	}
	
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UBattleTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	EXECUTE_ON_SERVER
	{
		if (bSearchBattleTarget)
		{
			SearchBattleTarget();
		}
		else
		{
			LoseBattleTarget(ELoseTargetType::E_LoseTargetType_Normal);
		}

		UpdateAlert(DeltaTime);
		CheckBackDistance();
	}
}

void UBattleTargetComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBattleTargetComponent, BattleTarget);
	DOREPLIFETIME(UBattleTargetComponent, AlertTarget);

	// FDoRepLifetimeParams repLifetimeParams;
	// repLifetimeParams.Condition = COND_OwnerOnly;
	// repLifetimeParams.RepNotifyCondition = REPNOTIFY_OnChanged;
	// repLifetimeParams.bIsPushBased = true;
	// DOREPLIFETIME_WITH_PARAMS(UBattleTargetComponent, BattleTarget, repLifetimeParams);
	//
	// DOREPLIFETIME_CONDITION(UBattleTargetComponent, BattleTarget, COND_OwnerOnly);
}

void UBattleTargetComponent::OnAIStateChanged(FGameplayTag& oldTag, FGameplayTag& newTag)
{
	if (newTag == AI_Monster_State_Death)
	{
		bSearchBattleTarget = false;
		bSearchAlertTarget = false;
		return;
	}
	//back状态不搜寻目标
	if (newTag == AI_Monster_State_Back)
	{
		bSearchBattleTarget = false;
	}
	else if (oldTag == AI_Monster_State_Back)
	{
		bSearchBattleTarget = true;
	}
	
	//Find状态搜寻目标不需要警戒范围触发
	bSearchAlertTarget = !(newTag == AI_Monster_State_Find);
}

void UBattleTargetComponent::OnHurt(int32 sponsorId, int32 targetId)
{
	if (Owner->GetID() != targetId)
	{
		return;
	}
	UGHCharacterMgr* characterMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CharacterMgr;
	AGHBaseCharacter* sponsor = characterMgr->GetCharacter(sponsorId);
	if (sponsor != nullptr)
	{
		SetBattleTarget(sponsor);
	}
}

AGHBaseCharacter* UBattleTargetComponent::GetBattleTarget()
{
	return BattleTarget;
}

void UBattleTargetComponent::SetBattleTarget(AGHBaseCharacter* target)
{
	EXECUTE_ON_CLIENT
	{
		return;
	}
	
	if (target == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GH---BattleTargetComponent::SetBattleTarget: target is null!!!"));
		return;
	}
	//暂时不考虑切换目标
	if (BattleTarget != nullptr)
	{
		return;
	}
	BattleTarget = target;

	UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
	coreDelegatesMgr->OnBattleSearchTarget.Execute(BattleTarget->GetID());
}

void UBattleTargetComponent::LoseBattleTarget(ELoseTargetType loseType)
{
	EXECUTE_ON_CLIENT
	{
		return;
	}
	if (BattleTarget == nullptr)
	{
		return;
	}
	
	BattleTarget = nullptr;

	UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
	coreDelegatesMgr->OnBattleLoseTarget.Execute(loseType);
}

void UBattleTargetComponent::SearchBattleTarget()
{
	EXECUTE_ON_CLIENT
	{
		return;
	}
	//目标不合法才需要重新查找目标，demo暂不支持切换目标的逻辑
	if (CheckTargetValid())
	{
		return;
	}

	TArray<AGHBasePlayer*> allPlayers;
	UGHCharacterMgr* characterMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CharacterMgr;
	characterMgr->GetAllPlayers(allPlayers);
	AGHBasePlayer* tempTarget = nullptr;
	AGHBasePlayer* tempAlertTarget = nullptr;
	for (auto& player : allPlayers)
	{
		if (!player->IsValidLowLevelFast())
		{
			continue;
		}
		
		float dis = UGHCommonUtils::CalcDistance(Owner, player);
		if (dis > SearchTargetWarnMaxDistance)
		{
			continue;
		}

		float angle = UGHCommonUtils::Calc2DAngleByForward(Owner, player);
		if (angle > (SearchTargetAngle / 2))
		{
			if (bSearchAlertTarget && tempAlertTarget == nullptr)
			{
				tempAlertTarget = player;
			}
			continue;
		}
		//todo 选到目标就直接退出，暂不考虑择优选择
		tempTarget = player;
		tempAlertTarget = nullptr;//选到目标清空警戒目标
		break;
	}

	//只要找到警戒目标就开始警戒，若找到目标则会停止警戒
	if (AlertTarget == nullptr && tempAlertTarget != nullptr)
	{
		SetAlertTarget(tempAlertTarget);
	}
	else if (AlertTarget != nullptr && tempAlertTarget == nullptr)
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
		LoseBattleTarget(ELoseTargetType::E_LoseTargetType_Normal);
	}
}

bool UBattleTargetComponent::CheckTargetValid()
{
	if (BattleTarget == nullptr)
	{
		return false;
	}

	float dis = UGHCommonUtils::CalcDistance(Owner, BattleTarget);
	return dis < LoseTargetDistance;
}

void UBattleTargetComponent::StartAlert()
{
	EXECUTE_ON_CLIENT
	{
		return;
	}
	if (bAlert)
	{
		return;
	}
	bAlert = true;
	
	UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
	coreDelegatesMgr->OnStartAlert.ExecuteIfBound();
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
		UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
		coreDelegatesMgr->OnFinishAlert.ExecuteIfBound();
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
	EXECUTE_ON_CLIENT
	{
		return;
	}
	if (alertTarget != nullptr)
	{
		StartAlert();
	}
	AlertTarget = alertTarget;
}

void UBattleTargetComponent::CheckBackDistance()
{
	float distance = UGHCommonUtils::CalcDistance2(Owner, Owner->BornLocation);
	if (distance > BackDistance)
	{
		LoseBattleTarget(ELoseTargetType::E_LoseTargetType_Back);
	}
}
