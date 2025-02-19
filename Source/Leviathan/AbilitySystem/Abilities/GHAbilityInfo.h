#pragma once
#include "GHGameplayAbility.h"
#include "GHAbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FGHAbilityInfo
{
	GENERATED_BODY()
	//技能
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Level;

	//技能ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SkillID;
};

UCLASS(BlueprintType)
class UGHAbilityInfoList : public UDataAsset
{
	GENERATED_BODY()

public:
	//技能
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGHAbilityInfo> AbilityInfoList;
};

USTRUCT(BlueprintType)
struct FSkillField : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> AnimPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGHGameplayAbility> Ability;
};
