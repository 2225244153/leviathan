#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Leviathan/GHGameFrameWork/GHGameInstace.h"
#include "GHCoreDelegatesMgr.generated.h"

DECLARE_DELEGATE_OneParam(FOnBattleSearchTarget, int32)
DECLARE_DELEGATE_OneParam(FOnBattleLoseTarget, uint8)
DECLARE_DELEGATE(FOnStartAlert)
DECLARE_DELEGATE(FOnFinishAlert)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIStateChanged, FGameplayTag&, oldTag, FGameplayTag&, newTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterHurt, int32, sponsorId, int32, targetId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDeath, int32, sponsorId, int32, targetId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterEndAbility, int32, id, bool, bWasCancelled);

UCLASS()
class LEVIATHAN_API UGHCoreDelegatesMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGHCoreDelegatesMgr();

	void ClearDelegates();
	/*
	 *	BattleTargetComponent发现目标广播
	 */
	FOnBattleSearchTarget OnBattleSearchTarget;
	/*
	 *  BattleTargetComponent丢失目标广播
	 */
	FOnBattleLoseTarget OnBattleLoseTarget;
	/*
	 *	BattleTargetComponent进入警戒状态
	 */
	FOnStartAlert OnStartAlert;
	/*
	 *  BattleTargetComponent离开警戒状态(警戒状态发现目标不会调用)
	 */
	FOnFinishAlert OnFinishAlert;
	/*
	 *	AIStateComponent状态发生改变
	 */
	UPROPERTY(BlueprintAssignable, Category = "GHCoreDelegates")
	FOnAIStateChanged OnAIStateChanged;
	/*
	 *	角色受击
	 *	@param1: 发起方id
	 *	@param2: 受击方id
	 */
	UPROPERTY(BlueprintAssignable, Category = "GHCoreDelegates")
	FOnCharacterHurt OnCharacterHurt;
	/*
	 *	角色死亡
	 *	@param1: 发起方id
	 *	@param2: 死亡方id
	 */
	UPROPERTY(BlueprintAssignable, Category = "GHCoreDelegates")
	FOnCharacterDeath OnCharacterDeath;
	/*
	 *  技能结束
	 *  @param1: 释放者id
	 *  @param2：是否被取消
	 */
	UPROPERTY(BlueprintAssignable, Category = "GHCoreDelegates")
	FOnCharacterEndAbility OnCharacterEndAbility;
};
