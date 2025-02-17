// Copyright Relink Games, Inc. All Rights Reserved. 


#include "GHAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "Leviathan/AbilitySystem/GHGameplayTags.h"

UGHAssetManager::UGHAssetManager()
{
}

UGHAssetManager& UGHAssetManager::Get()
{
	check(GEngine);

	if (UGHAssetManager *singleton = Cast<UGHAssetManager>(GEngine->AssetManager))
	{
		return *singleton;
	}

	return *NewObject<UGHAssetManager>();
}

void UGHAssetManager::StartInitialLoading()
{
    UGameplayTagsManager &manager = UGameplayTagsManager::Get();

	// 先初始化NativeTags
	FGHGameplayTags::Get().InitNativeTags();
	
    // 加载资源
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();

    // 结束NativeTags的初始化
    manager.DoneAddingNativeTags();
}
