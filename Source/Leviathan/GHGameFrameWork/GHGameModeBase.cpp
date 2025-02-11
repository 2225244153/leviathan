// Fill out your copyright notice in the Description page of Project Settings.


#include "GHGameModeBase.h"

#include "GHBasePlayerController.h"

AGHGameModeBase::AGHGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GH_Work/Character/Default/Blueprints/BP_Player_Default"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = AGHBasePlayerController::StaticClass();
}
