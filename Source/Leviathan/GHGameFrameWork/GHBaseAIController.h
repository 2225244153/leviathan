// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "GHBaseAIController.generated.h"

class AGHBaseMonster;

/*
 * 怪物状态
 */
UENUM(BlueprintType)
enum class EAIMonsterState : uint8
{
	E_AIMonsterState_None		UMETA(DisplayName = "None"),
	E_AIMonsterState_Init		UMETA(DisplayName = "Init"),
	E_AIMonsterState_Alert		UMETA(DisplayName = "Alert"),
	E_AIMonsterState_Battle		UMETA(DisplayName = "Battle"),
	E_AIMonsterState_Find		UMETA(DisplayName = "Find"),
	E_AIMonsterState_Back		UMETA(DisplayName = "Back"),
	E_AIMonsterState_Death		UMETA(DisplayName = "Death"),
};

/*
 * 怪物战斗状态
 */
UENUM(BlueprintType)
enum class EAIMonsterBattleState : uint8
{
	E_AIMonsterState_BattleNone		UMETA(DisplayName = "BattleNone"),
	E_AIMonsterState_BattlePursue	UMETA(DisplayName = "BattlePursue"),
	E_AIMonsterState_BattleTurn		UMETA(DisplayName = "BattleTurn"),
	E_AIMonsterState_BattleAttack	UMETA(DisplayName = "BattleAttack"),
	E_AIMonsterState_BattlePerform	UMETA(DisplayName = "BattlePerform"),
	E_AIMonsterState_BattleHurt		UMETA(DisplayName = "BattleHurt"),
};

UCLASS()
class LEVIATHAN_API AGHBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGHBaseAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	UPROPERTY()
	AGHBaseMonster* BaseMonster;
};
