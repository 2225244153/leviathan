// Fill out your copyright notice in the Description page of Project Settings.


#include "GHCharacterMgr.h"

#include "Leviathan/GHGameFrameWork/GHBaseCharacter.h"
#include "Leviathan/GHGameFrameWork/GHBaseMonster.h"
#include "Leviathan/GHGameFrameWork/GHBasePlayer.h"

UGHGameInstace* UGHCharacterMgr::Inst = nullptr;

UGHCharacterMgr::UGHCharacterMgr()
{
}

UGHCharacterMgr* UGHCharacterMgr::Get()
{
	return UGameInstance::GetSubsystem<UGHCharacterMgr>(Inst);
}

void UGHCharacterMgr::GHInit(UGHGameInstace* inst)
{
	Inst = inst;
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
	AllCharacters.Emplace(baseCharacter->GetID(), baseCharacter);
	AGHBasePlayer* basePlayer = Cast<AGHBasePlayer>(SpawnedActor);
	if (basePlayer != nullptr)
	{
		AllPlayers.Emplace(basePlayer);
	}
	AGHBaseMonster* baseMonster = Cast<AGHBaseMonster>(SpawnedActor);
	if (baseMonster != nullptr)
	{
		AllMonsters.Emplace(baseMonster);
	}
}

void UGHCharacterMgr::OnActorDestroyed(AActor* DestroyedActor)
{
	if (DestroyedActor == nullptr || Cast<AGHBaseCharacter>(DestroyedActor) == nullptr)
	{
		return;
	}

	AGHBaseCharacter* baseCharacter = Cast<AGHBaseCharacter>(DestroyedActor);
	AllCharacters.Remove(baseCharacter->GetID());
	AGHBasePlayer* basePlayer = Cast<AGHBasePlayer>(DestroyedActor);
	if (basePlayer != nullptr)
	{
		AllPlayers.RemoveSwap(basePlayer);
	}
	AGHBaseMonster* baseMonster = Cast<AGHBaseMonster>(DestroyedActor);
	if (baseMonster != nullptr)
	{
		AllMonsters.RemoveSwap(baseMonster);
	}
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
