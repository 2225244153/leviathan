// Fill out your copyright notice in the Description page of Project Settings.


#include "GHGameInstace.h"

#include "Leviathan/GHManagers/GHCharacterMgr.h"
#include "Leviathan/GHManagers/GHCoreDelegatesMgr.h"

UGHGameInstace::UGHGameInstace()
{
}

UGHGameInstace::~UGHGameInstace()
{
}

void UGHGameInstace::GHInit()
{
	UGHCharacterMgr* characterMgr = GetSubsystem<UGHCharacterMgr>(this);
	if (characterMgr != nullptr)
	{
		characterMgr->GHInit(this);
	}
	UGHCoreDelegatesMgr* coreDelegatesMgr = GetSubsystem<UGHCoreDelegatesMgr>(this);
	if (coreDelegatesMgr != nullptr)
	{
		coreDelegatesMgr->GHInit(this);
	}
}

void UGHGameInstace::StartGameInstance()
{
	Super::StartGameInstance();

	GHInit();
}

void UGHGameInstace::Shutdown()
{
	UGHCoreDelegatesMgr::Get()->ClearDelegates();
	
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
