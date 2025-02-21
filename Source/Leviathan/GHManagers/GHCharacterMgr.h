// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Leviathan/GHGameFrameWork/GHGameInstace.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GHCharacterMgr.generated.h"

class AGHBaseMonster;
class AGHBasePlayer;
class AGHBaseCharacter;

/**
 * 角色管理
 * @company GH
 * @author hsp
 * @date 2025-2-14
 */
UCLASS()
class LEVIATHAN_API UGHCharacterMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGHCharacterMgr();

	void GHInit();
	
	void virtual Initialize(FSubsystemCollectionBase& Collection) override;
	void virtual Deinitialize() override;
	
	void OnActorSpawned(AActor* SpawnedActor);
	void OnActorDestroyed(AActor* DestroyedActor);

	UFUNCTION(BlueprintCallable)
	void GetAllCharacters(TMap<int32, AGHBaseCharacter*>& allCharacters);
	UFUNCTION(BlueprintCallable)
	void GetAllPlayers(TArray<AGHBasePlayer*>& allPlayers);
	UFUNCTION(BlueprintCallable)
	void GetAllMonsters(TArray<AGHBaseMonster*>& allMonsters);
	UFUNCTION(BlueprintCallable)
	AGHBaseCharacter* GetCharacter(int32 id);

	void RegisterCharacter(AGHBaseCharacter* character);
	void UnregisterCharacter(AGHBaseCharacter* character);
	
private:
	UPROPERTY()
	TMap<int32, AGHBaseCharacter*> AllCharacters;
	UPROPERTY()
	TArray<AGHBasePlayer*> AllPlayers;
	UPROPERTY()
	TArray<AGHBaseMonster*> AllMonsters;

	FDelegateHandle OnActorSpawnedDelegateHandle;
	FDelegateHandle OnActorDestroyedDelegateHandle;

};
