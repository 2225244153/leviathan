// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GHGameInstace.generated.h"

/**
 * 
 */
UCLASS()
class LEVIATHAN_API UGHGameInstace : public UGameInstance
{
	GENERATED_BODY()

public:
	UGHGameInstace();
	~UGHGameInstace();

	void GHInit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=GHGameInstace)
	class UGHCharacterMgr* CharacterMgr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=GHGameInstace)
	class UGHCoreDelegatesMgr* CoreDelegatesMgr;
	
protected:
	virtual void StartGameInstance() override;
	virtual void Shutdown() override;

#if WITH_EDITOR
	virtual FGameInstancePIEResult StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params) override;
#endif

	/*
	 * Debug
	 */
public:
	FTimerHandle TickTimerHandle;
	
	void TickTimer();
	
	/*
	 *	debug怪物
	 *	@param type 1-显示怪物警戒范围 2-显示怪物跟随范围 3-显示怪物出生点范围
	 */
	UFUNCTION(Exec)
	void GHDebugAIMonster(int32 type, int32 active);

	bool bDebugTargetRange;//怪物警戒范围
	bool bDebugAIPurse;//怪物跟随范围
	bool bDebugBornRange;//怪物出生点范围，超过此返回back
};
