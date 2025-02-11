// Fill out your copyright notice in the Description page of Project Settings.


#include "GHGameInstace.h"

UGHGameInstace::UGHGameInstace()
{
}

UGHGameInstace::~UGHGameInstace()
{
}

void UGHGameInstace::StartGameInstance()
{
	Super::StartGameInstance();
}

void UGHGameInstace::Shutdown()
{
	Super::Shutdown();
}

#if WITH_EDITOR
FGameInstancePIEResult UGHGameInstace::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params)
{
	return Super::StartPlayInEditorGameInstance(LocalPlayer, Params);
}
#endif
