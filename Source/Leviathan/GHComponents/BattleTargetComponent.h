// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleTargetComponent.generated.h"

class AGHBaseCharacter;

/*
 * 查找战斗目标组件，默认该组件只绑定BaseMonster，只会查找BasePlayer为目标
 * 角色查找怪物为攻击目标不支持 todo
 * 暂时不支持切换目标的逻辑
 * @company GH
 * @author hsp
 * @date 2025-2-14
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEVIATHAN_API UBattleTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBattleTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	AGHBaseCharacter* GetBattleTarget();
	//todo监听受击直接设置攻击者为目标
	
	UFUNCTION(BlueprintCallable)
	void ResetBattleTarget();

	/*
	 *	寻找目标
	 */
	void FindBattleTarget();
	/*
	 * 判断目标是否合法 目标为空或者超过范围则为不合法
	 */
	bool CheckTargetValid();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattleTarget")
	bool bFindBattleTarget;
	
private:
	UPROPERTY()
	AGHBaseCharacter* BattleTarget;
	UPROPERTY()
	AGHBaseCharacter* Owner;
};
