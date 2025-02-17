#pragma once
#include "GHGameplayAbility.h"
#include "GHAbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FGHAbilityInfo
{
	GENERATED_BODY()
	//技能
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGHGameplayAbility> AbilityClass;

	//技能
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Level;
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
