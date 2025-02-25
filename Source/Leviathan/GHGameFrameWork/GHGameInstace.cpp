// Fill out your copyright notice in the Description page of Project Settings.


#include "GHGameInstace.h"

#include "GHBaseAIController.h"
#include "GHBaseMonster.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Leviathan/GameDefine.h"
#include "Leviathan/GHComponents/BattleTargetComponent.h"
#include "Leviathan/GHManagers/GHCharacterMgr.h"
#include "Leviathan/GHManagers/GHCoreDelegatesMgr.h"
#include "Leviathan/GHUtils/SkillUtils.h"

UGHGameInstace::UGHGameInstace()
{
}

UGHGameInstace::~UGHGameInstace()
{
}

void UGHGameInstace::GHInit()
{
	CharacterMgr = GetSubsystem<UGHCharacterMgr>(this);
	if (CharacterMgr != nullptr)
	{
		CharacterMgr->GHInit();
	}
	CoreDelegatesMgr = GetSubsystem<UGHCoreDelegatesMgr>(this);
	
	if (GetWorld()->IsNetMode(NM_Client))
	{
		GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UGHGameInstace::TickTimer, 0.02, true);
	}
}

void UGHGameInstace::StartGameInstance()
{
	GHInit();
	
	Super::StartGameInstance();
}

void UGHGameInstace::Shutdown()
{
	if (CoreDelegatesMgr != nullptr)
	{
		CoreDelegatesMgr->ClearDelegates();
	}
	USkillUtils::ClearCacheDatas();
	if (TickTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	}
	
	Super::Shutdown();
}

#if WITH_EDITOR
FGameInstancePIEResult UGHGameInstace::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params)
{
	GHInit();
	FGameInstancePIEResult result = Super::StartPlayInEditorGameInstance(LocalPlayer, Params);
	return result;
}
#endif

void UGHGameInstace::TickTimer()
{
	if (bDebugTargetRange || bDebugAIPurse || bDebugBornRange)
	{
		TArray<AActor*> monsters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGHBaseMonster::StaticClass(), monsters);
		for (auto& tempMonster : monsters)
		{
			AGHBaseMonster* monster = Cast<AGHBaseMonster>(tempMonster);
			if (bDebugTargetRange)
			{
				float findTargetAngle = monster->TargetComponent->SearchTargetAngle;
				float findTargetWarnMaxDistance = monster->TargetComponent->SearchTargetWarnMaxDistance;
				float angleRadian = FMath::DegreesToRadians(findTargetAngle / 2);
				DrawDebugCone(monster->GetWorld(), monster->GetActorLocation(), monster->GetActorForwardVector(), findTargetWarnMaxDistance, angleRadian, 0, 12, FColor::Red, false, 0.2);
				DrawDebugCircle(GetWorld(), monster->GetActorLocation(), findTargetWarnMaxDistance, 36, FColor::Green, false, 0.02, 0, 0, FVector(1, 0, 0), FVector(0, 1, 0), false);
			}
			if (bDebugAIPurse)
			{
				float loseTargetDistance = monster->TargetComponent->LoseTargetDistance;
				DrawDebugCircle(GetWorld(), monster->GetActorLocation(), loseTargetDistance, 36, FColor::Purple, false, 0.02, 0, 0, FVector(1, 0, 0), FVector(0, 1, 0), false);
			}
			if (bDebugBornRange)
			{
				float backDistance = monster->TargetComponent->BackDistance;
				FVector location = FVector(monster->BornLocation.X, monster->BornLocation.Y, monster->BornLocation.Z - monster->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 10.f);
				DrawDebugCircle(GetWorld(), location, backDistance, 36, FColor::Red, false, 0.02, 0, 0, FVector(1, 0, 0), FVector(0, 1, 0), false);
			}
		}
	}
}

void UGHGameInstace::GHDebugAIMonster(int32 type, int32 active)
{
	switch (type)
	{
	case 1:
		bDebugTargetRange = (bool)active;
		break;
	case 2:
		bDebugAIPurse = (bool)active;
		break;
	case 3:
		bDebugBornRange = (bool)active;
		break;
	default:
		break;
	}
}
