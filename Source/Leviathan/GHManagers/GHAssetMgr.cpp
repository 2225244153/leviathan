

#include "GHAssetMgr.h"
#include "AbilitySystemGlobals.h"
#include "Leviathan/AbilitySystem/GHGameplayTags.h"

UGHAssetMgr::UGHAssetMgr()
{
}

UGHAssetMgr& UGHAssetMgr::Get()
{
	check(GEngine);

	if (UGHAssetMgr *singleton = Cast<UGHAssetMgr>(GEngine->AssetManager))
	{
		return *singleton;
	}

	return *NewObject<UGHAssetMgr>();
}

void UGHAssetMgr::StartInitialLoading()
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
