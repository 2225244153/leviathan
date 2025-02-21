// Fill out your copyright notice in the Description page of Project Settings.


#include "GHCharacterMgr.h"

#include "EngineUtils.h"
#include "Leviathan/GHGameFrameWork/GHBaseCharacter.h"
#include "Leviathan/GHGameFrameWork/GHBaseMonster.h"
#include "Leviathan/GHGameFrameWork/GHBasePlayer.h"

UGHCharacterMgr::UGHCharacterMgr()
{
}

void UGHCharacterMgr::GHInit()
{
	// 扫描当前场景中的所有 Actor
	UWorld* World = GetWorld();
	if (World)
	{
		for (TActorIterator<AActor> It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor || !Actor->IsA(AGHBaseCharacter::StaticClass()))
			{
				continue;
			}

			AGHBaseCharacter* baseCharacter = Cast<AGHBaseCharacter>(Actor);
			RegisterCharacter(baseCharacter);
		}
	}
}

void UGHCharacterMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FOnActorSpawned::FDelegate ActorSpawnedDelegate = FOnActorSpawned::FDelegate::CreateUObject(this, &UGHCharacterMgr::OnActorSpawned);
	OnActorSpawnedDelegateHandle = GetWorld()->AddOnActorSpawnedHandler(ActorSpawnedDelegate);
	const FOnActorDestroyed::FDelegate ActorDestroyedDelegate = FOnActorDestroyed::FDelegate::CreateUObject(this, &UGHCharacterMgr::OnActorDestroyed);
	OnActorDestroyedDelegateHandle = GetWorld()->AddOnActorDestroyedHandler(ActorDestroyedDelegate);
}

void UGHCharacterMgr::Deinitialize()
{
	Super::Deinitialize();

	GetWorld()->RemoveOnActorSpawnedHandler(OnActorSpawnedDelegateHandle);
	GetWorld()->RemoveOnActorDestroyededHandler(OnActorDestroyedDelegateHandle);

	AllCharacters.Empty();
	AllPlayers.Empty();
	AllMonsters.Empty();
}

void UGHCharacterMgr::OnActorSpawned(AActor* SpawnedActor)
{
	if (SpawnedActor == nullptr || Cast<AGHBaseCharacter>(SpawnedActor) == nullptr)
	{
		return;
	}

	AGHBaseCharacter* baseCharacter = Cast<AGHBaseCharacter>(SpawnedActor);
	RegisterCharacter(baseCharacter);
}

void UGHCharacterMgr::OnActorDestroyed(AActor* DestroyedActor)
{
	if (DestroyedActor == nullptr || Cast<AGHBaseCharacter>(DestroyedActor) == nullptr)
	{
		return;
	}

	AGHBaseCharacter* baseCharacter = Cast<AGHBaseCharacter>(DestroyedActor);
	UnregisterCharacter(baseCharacter);
}

void UGHCharacterMgr::GetAllCharacters(TMap<int32, AGHBaseCharacter*>& allCharacters)
{
	allCharacters = AllCharacters;
}

void UGHCharacterMgr::GetAllPlayers(TArray<AGHBasePlayer*>& allPlayers)
{
	allPlayers = AllPlayers;
}

void UGHCharacterMgr::GetAllMonsters(TArray<AGHBaseMonster*>& allMonsters)
{
	allMonsters = AllMonsters;
}

AGHBaseCharacter* UGHCharacterMgr::GetCharacter(int32 id)
{
	return AllCharacters.FindRef(id);
}

void UGHCharacterMgr::RegisterCharacter(AGHBaseCharacter* character)
{
	if (AllCharacters.Contains(character->GetID()))
	{
		UE_LOG(LogTemp, Warning, TEXT("GH---UGHCharacterMgr::RegisterCharacter is already register!!!"));
		return;
	}
	AllCharacters.Emplace(character->GetID(), character);
	AGHBasePlayer* basePlayer = Cast<AGHBasePlayer>(character);
	if (basePlayer != nullptr)
	{
		AllPlayers.Emplace(basePlayer);
	}
	AGHBaseMonster* baseMonster = Cast<AGHBaseMonster>(character);
	if (baseMonster != nullptr)
	{
		AllMonsters.Emplace(baseMonster);
	}
}

void UGHCharacterMgr::UnregisterCharacter(AGHBaseCharacter* character)
{
	AGHBaseCharacter* baseCharacter = Cast<AGHBaseCharacter>(character);
	AllCharacters.Remove(baseCharacter->GetID());
	AGHBasePlayer* basePlayer = Cast<AGHBasePlayer>(character);
	if (basePlayer != nullptr)
	{
		AllPlayers.RemoveSwap(basePlayer);
	}
	AGHBaseMonster* baseMonster = Cast<AGHBaseMonster>(character);
	if (baseMonster != nullptr)
	{
		AllMonsters.RemoveSwap(baseMonster);
	}
}
