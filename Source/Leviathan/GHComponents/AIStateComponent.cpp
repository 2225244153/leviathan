
#include "AIStateComponent.h"

#include "BattleTargetComponent.h"
#include "Leviathan/GameDefine.h"
#include "Leviathan/GHGameFrameWork/GHBaseMonster.h"
#include "Leviathan/GHManagers/GHCharacterMgr.h"
#include "Leviathan/GHManagers/GHCoreDelegatesMgr.h"
#include "Leviathan/GHUtils/GHCommonUtils.h"
#include "Net/UnrealNetwork.h"

UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Init, "AI.State.Init");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Alert, "AI.State.Alert");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle, "AI.State.Battle");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Find, "AI.State.Find");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Back, "AI.State.Back");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Death, "AI.State.Death");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Pursue, "AI.State.Battle.Pursue");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Turn, "AI.State.Battle.Turn");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Attack, "AI.State.Battle.Attack");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Perform, "AI.State.Battle.Perform");
UE_DEFINE_GAMEPLAY_TAG(AI_Monster_State_Battle_Hurt, "AI.State.Battle.Hurt");

// Sets default values for this component's properties
UAIStateComponent::UAIStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAIStateComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AGHBaseMonster>(GetOwner());

	EXECUTE_ON_SERVER
	{
		UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
		coreDelegatesMgr->OnBattleSearchTarget.BindUObject(this, &UAIStateComponent::OnBattleSearchTarget);
		coreDelegatesMgr->OnBattleLoseTarget.BindUObject(this, &UAIStateComponent::OnBattleLoseTarget);
		coreDelegatesMgr->OnStartAlert.BindUObject(this, &UAIStateComponent::OnStartAlert);
		coreDelegatesMgr->OnFinishAlert.BindUObject(this, &UAIStateComponent::OnFinishAlert);
		coreDelegatesMgr->OnCharacterDeath.AddUniqueDynamic(this, &UAIStateComponent::OnDeath);
		coreDelegatesMgr->OnCharacterHurt.AddUniqueDynamic(this, &UAIStateComponent::OnHurt);

		SetState(AI_Monster_State_Init);
	}
}

void UAIStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EXECUTE_ON_SERVER
	{
		UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
    	coreDelegatesMgr->OnBattleSearchTarget.Unbind();
    	coreDelegatesMgr->OnBattleLoseTarget.Unbind();
    	coreDelegatesMgr->OnStartAlert.Unbind();
    	coreDelegatesMgr->OnFinishAlert.Unbind();
    	coreDelegatesMgr->OnCharacterDeath.RemoveDynamic(this, &UAIStateComponent::OnDeath);
    	coreDelegatesMgr->OnCharacterHurt.RemoveDynamic(this, &UAIStateComponent::OnHurt);
	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UAIStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	EXECUTE_ON_SERVER
	{
		if (bFinding)
		{
			UpdateFindingTime(DeltaTime);
		}
		if (TagState == AI_Monster_State_Back)
		{
			CheckBackBornLocation(DeltaTime);
		}
	}
}

void UAIStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAIStateComponent, TagState);
}

void UAIStateComponent::OnBattleSearchTarget(int32 targetId)
{
	UGHCharacterMgr* characterMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CharacterMgr;
	AGHBaseCharacter* target = characterMgr->GetCharacter(targetId);
	if (target == nullptr)
	{
		return;
	}
	
	//AI_Monster_State_Find状态重新找到目标直接结束Finding
	FinishFinding();
	
	SetState(AI_Monster_State_Battle);
}

void UAIStateComponent::OnBattleLoseTarget(uint8 loseType)
{
	switch (loseType)
	{
	case E_LoseTargetType_Normal:
		SetState(AI_Monster_State_Find);
		break;
	case E_LoseTargetType_Back:
		SetState(AI_Monster_State_Back);
		break;
	default:
		break;
	}
}

void UAIStateComponent::OnStartAlert()
{
	SetState(AI_Monster_State_Alert);
}

void UAIStateComponent::OnFinishAlert()
{
	SetState(AI_Monster_State_Init);
}

void UAIStateComponent::OnDeath(int32 sponsorId, int32 targetId)
{
	if (Owner->GetID() != targetId)
	{
		return;
	}
	SetState(AI_Monster_State_Death);
}

void UAIStateComponent::OnHurt(int32 sponsorId, int32 targetId)
{
	if (Owner->GetID() != targetId)
	{
		return;
	}

	SetState(AI_Monster_State_Battle_Hurt);
}

FGameplayTag UAIStateComponent::GetState()
{
	return TagState;
}

void UAIStateComponent::SetState(FGameplayTag state)
{
	EXECUTE_ON_CLIENT
	{
		return;
	}
	if (TagState.IsValid())
	{
		FGameplayTagContainer tagContainer = TagState.GetGameplayTagParents();
		if (tagContainer.HasTag(state))
		{
			return;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("GH------UAIStateComponent::SetState: %s"), *state.ToString());
	FGameplayTag oldTag = TagState;
	TagState = state;

	UGHCoreDelegatesMgr* coreDelegatesMgr = Cast<UGHGameInstace>(GetWorld()->GetGameInstance())->CoreDelegatesMgr;
	coreDelegatesMgr->OnAIStateChanged.Broadcast(oldTag, TagState);

	if (state == AI_Monster_State_Find)
	{
		StartFinding();
	}
	else if (state == AI_Monster_State_Back)
	{
		CurBackTime = 0;
	}
}

void UAIStateComponent::StartFinding()
{
	CurFindingTime = 0;
	bFinding = true;
}

void UAIStateComponent::FinishFinding()
{
	CurFindingTime = 0;
	bFinding = false;
}

void UAIStateComponent::UpdateFindingTime(float DeltaTime)
{
	if (CurFindingTime > MaxFindingTime)
	{
		FinishFinding();
		SetState(AI_Monster_State_Back);
		return;
	}
	
	CurFindingTime += DeltaTime;
}

void UAIStateComponent::CheckBackBornLocation(float DeltaTime)
{
	//超过20s直接传送回出生点
	if (CurBackTime > 20.f)
	{
		CurBackTime = 0;
		Owner->SetActorLocation(Owner->BornLocation);
		SetState(AI_Monster_State_Init);
		return;
	}
	float distance = UGHCommonUtils::CalcDistance2(Owner, Owner->BornLocation);
	if (distance < 100.f)
	{
		CurBackTime = 0;
		SetState(AI_Monster_State_Init);
		return;
	}

	CurBackTime += DeltaTime;
}
