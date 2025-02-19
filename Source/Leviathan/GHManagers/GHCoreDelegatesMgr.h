#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Leviathan/GHGameFrameWork/GHBaseCharacter.h"
#include "GHCoreDelegatesMgr.generated.h"

UCLASS()
class LEVIATHAN_API UGHCoreDelegatesMgr : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static void ClearDelegates();
	/*
	 *	BattleTargetComponent发现目标广播
	 */
	static TMulticastDelegate<void(AGHBaseCharacter*)> OnBattleSearchTarget;
	/*
	 *  BattleTargetComponent丢失目标广播
	 */
	static TMulticastDelegate<void(uint8)> OnBattleLoseTarget;
	/*
	 *	BattleTargetComponent进入警戒状态
	 */
	static TDelegate<void()> OnStartAlert;
	/*
	 *  BattleTargetComponent离开警戒状态(警戒状态发现目标不会调用)
	 */
	static TDelegate<void()> OnFinishAlert;

	/*
	 *	AIStateComponent状态发生改变
	 */
	static TMulticastDelegate<void(FGameplayTag&, FGameplayTag&)> OnAIStateChanged;
};
