// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SkillUtils.generated.h"

class UGHGameplayAbility;

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGHGameplayAbility> Ability;

	bool IsValid() const
	{
		return ID != 0;
	}
};

/**
 * 
 */
UCLASS()
class LEVIATHAN_API USkillUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void ClearCacheDatas();
	
	UFUNCTION(BlueprintCallable, Category = "GH|SkillUtils")
	static FSkillData GetSkillDataBySkillID(int32 skillID);

private:
	static TMap<int32, FSkillData> SkillDatas;
};
