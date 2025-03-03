// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags.h"
#include "GHAnimationLib.generated.h"

USTRUCT(BlueprintType)
struct FGHAnimMontageInfo
{
	GENERATED_BODY()

	/**
	 * @brief 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Montage")
	TSoftObjectPtr<UAnimMontage> AnimMontage;

	/**
	 * @brief 
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Montage")
	float PlayRate = 1.0f;
};

/**
 * 
 */
UCLASS()
class LEVIATHAN_API UGHAnimationLib : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief 保存动画蒙太奇
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim Montage")
	TMap<FGameplayTag, FGHAnimMontageInfo> AnimMontages;
};
