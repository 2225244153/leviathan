#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UObject/Object.h"
#include "GHAssetMgr.generated.h"

/**
 *
 * 资源管理
 * 这个类在DefaultEngine.ini文件中通过AssetManagerClassName字段指定
 */
UCLASS()
class LEVIATHAN_API UGHAssetMgr : public UAssetManager
{
	GENERATED_BODY()
public:
	UGHAssetMgr();
	static UGHAssetMgr& Get();

public:
	virtual void StartInitialLoading() override;
	
};
