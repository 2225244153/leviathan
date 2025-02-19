// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GHGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LEVIATHAN_API AGHGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGHGameModeBase();

	FTimerHandle TickTimerHandle;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void TickTimer();

	UFUNCTION(BlueprintCallable, Exec)
	void GHDebugPauseAI(int32 active);
	/*
	 *	debug怪物
	 *	@param type 1-显示怪物警戒范围 2-显示怪物跟随范围 3-显示怪物出生点范围
	 */
	UFUNCTION(BlueprintCallable, Exec)
	void GHDebugAIMonster(int32 type, int32 active);

	bool bDebugTargetRange;//怪物警戒范围
	bool bDebugAIPurse;//怪物跟随范围
	bool bDebugBornRange;//怪物出生点范围，超过此返回back
};
