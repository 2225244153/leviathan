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
	CharacterMgr = GetSubsystem<UGHCharacterMgr>(this);
	if (CharacterMgr != nullptr)
	{
		CharacterMgr->GHInit();
	}
	CoreDelegatesMgr = GetSubsystem<UGHCoreDelegatesMgr>(this);
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
