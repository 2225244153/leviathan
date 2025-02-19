// Fill out your copyright notice in the Description page of Project Settings.


#include "GHGameModeBase.h"

#include "GHBaseAIController.h"
#include "GHBaseMonster.h"
#include "GHBasePlayerController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Leviathan/GameDefine.h"
#include "Leviathan/GHComponents/BattleTargetComponent.h"
#include "Leviathan/GHManagers/GHCharacterMgr.h"

AGHGameModeBase::AGHGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GH_Work/Character/Default/Blueprints/BP_Player_Default"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = AGHBasePlayerController::StaticClass();
}

void AGHGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &AGHGameModeBase::TickTimer, 0.2, true);
}

void AGHGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (TickTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	}
}

void AGHGameModeBase::TickTimer()
{
	if (bDebugTargetRange || bDebugAIPurse)
	{
		TArray<AGHBaseMonster*> monsters;
		UGHCharacterMgr::Get()->GetAllMonsters(monsters);
		for (auto& monster : monsters)
		{
			AGHBaseAIController* aiController = Cast<AGHBaseAIController>(monster->GetController());
			if (aiController == nullptr)
			{
				continue;
			}

			if (bDebugTargetRange)
			{
				float findTargetAngle = monster->TargetComponent->FindTargetAngle;
				float findTargetWarnMaxDistance = monster->TargetComponent->FindTargetWarnMaxDistance;
				float angleRadian = FMath::DegreesToRadians(findTargetAngle / 2);
				DrawDebugCone(monster->GetWorld(), monster->GetActorLocation(), monster->GetActorForwardVector(), findTargetWarnMaxDistance, angleRadian, 0, 12, FColor::Red, false, 0.2);
				DrawDebugCircle(GetWorld(), monster->GetActorLocation(), findTargetWarnMaxDistance, 36, FColor::Green, false, 0.2, 0, 0, FVector(1, 0, 0), FVector(0, 1, 0), false);
			}
			if (bDebugAIPurse)
			{
				float loseTargetDistance = monster->TargetComponent->LoseTargetDistance;
				DrawDebugCircle(GetWorld(), monster->GetActorLocation(), loseTargetDistance, 36, FColor::Purple, false, 0.2, 0, 0, FVector(1, 0, 0), FVector(0, 1, 0), false);
			}
		}
	}
}

void AGHGameModeBase::GHDebugPauseAI(int32 active)
{
	TArray<AGHBaseMonster*> monsters;
	UGHCharacterMgr::Get()->GetAllMonsters(monsters);
	for (auto& monster : monsters)
	{
		AGHBaseAIController* aiController = Cast<AGHBaseAIController>(monster->GetController());
		if (aiController == nullptr)
		{
			continue;
		}
		UBehaviorTreeComponent* btComponent = Cast<UBehaviorTreeComponent>(aiController->GetBrainComponent());
		if (btComponent == nullptr)
		{
			continue;
		}
		if (active)
		{
			btComponent->PauseLogic(TEXT("None"));
		}
		else
		{
			btComponent->ResumeLogic(TEXT("None"));
		}
	}
}

void AGHGameModeBase::GHDebugAIMonster(int32 type, int32 active)
{
	switch (type)
	{
	case 1:
		bDebugTargetRange = (bool)active;
		break;
	case 2:
		bDebugAIPurse = (bool)active;
		break;
	default:
		break;
	}
}
