// Copyright Relink Games, Inc. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UObject/Object.h"
#include "GHAssetManager.generated.h"

/**
 *
 * 资源管理
 * 这个类在DefaultEngine.ini文件中通过AssetManagerClassName字段指定
 */
UCLASS()
class LEVIATHAN_API UGHAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UGHAssetManager();
	static UGHAssetManager& Get();

public:
	virtual void StartInitialLoading() override;
	
};
