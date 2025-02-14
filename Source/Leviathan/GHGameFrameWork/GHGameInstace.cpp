// Fill out your copyright notice in the Description page of Project Settings.


#include "GHGameInstace.h"

#include "Leviathan/GHManagers/GHCharacterMgr.h"

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
}

void UGHGameInstace::StartGameInstance()
{
	Super::StartGameInstance();

	GHInit();
}

void UGHGameInstace::Shutdown()
{
	Super::Shutdown();
}

#if WITH_EDITOR
FGameInstancePIEResult UGHGameInstace::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params)
{
	FGameInstancePIEResult result = Super::StartPlayInEditorGameInstance(LocalPlayer, Params);
	GHInit();
	return result;
}
#endif
